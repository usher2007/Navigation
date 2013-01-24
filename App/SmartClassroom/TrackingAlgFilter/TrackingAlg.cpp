#include "stdafx.h"
#include "TrackingAlg.h"

TrackingAlg::TrackingAlg()
{
	frameIndex = 0;
}

int TrackingAlg::Update(Mat& frame)
{
	if(frame.empty())
		return -1;

	if(frameIndex == 0)
	{
		bgSubtractor(frame, gbmForeground, Utility::GBM_LEARNING_RATE);
		threshold(gbmForeground, gbmForeground, Utility::FG_LOW_THRESH, Utility::FG_UP_THRESH, THRESH_BINARY);
		frameIndex ++;
		return 0;
	}

	if(frameIndex%Utility::TRACK_INTERVAL != 0)
	{
		frameIndex++;
		return 0;
	}

	bgSubtractor(frame, gbmForeground, Utility::GBM_LEARNING_RATE);
	threshold(gbmForeground, gbmForeground, Utility::FG_LOW_THRESH, Utility::FG_UP_THRESH, THRESH_BINARY);

	gbmForeground = gbmForeground/255;
	vector<Point2f> baryCenters;
	CalcImageBaryCenters(gbmForeground, baryCenters);
	vector<Point2f>::iterator centerIter;
	personManager.ResetAllPersonStatus();

	for(centerIter=baryCenters.begin(); centerIter!=baryCenters.end(); ++centerIter)
	{
		personManager.ProcessBaryCenter(*centerIter);

	}
	personManager.Update();

	rectangle(frame, Utility::BEGIN_TRACKING_AREA, Scalar(255,0,0));
	rectangle(frame, Utility::STOP_TRACKING_AREA, Scalar(0,0,255));

	personManager.DrawPersons(frame);

	frameIndex ++;
}

int TrackingAlg::CalcImageBaryCenters(const Mat& img, vector<Point2f>& baryCenters)
{
	Mat doubleImg = Mat(img.rows, img.cols, CV_64F);
	img.convertTo(doubleImg, CV_64F);
	Mat leftOnes = Mat::ones(1, img.rows, CV_64F);
	Mat rightOnes = Mat::ones(img.cols, 1, CV_64F);
	Mat foreHistByCol = leftOnes*doubleImg;
	int maxCol = foreHistByCol.cols;
	Mat tmpRes = foreHistByCol*rightOnes;
	double avgForeHist = tmpRes.at<double>(0,0)/foreHistByCol.cols;
	vector<int> foreCandidates;
	for(int j=0; j<maxCol; j++)
	{
		if(foreHistByCol.at<double>(0,j) > Utility::FG_HIST_THRESH*avgForeHist)
		{
			foreCandidates.push_back(j);
		}
	}
	vector<int>::iterator it = foreCandidates.begin();
	int lastIndex = *it;
	int continueNum = 0;
	double sumWeight = 0;
	double eachColSum = 0;
	for(; it != foreCandidates.end(); it ++)
	{
		if((*it) - lastIndex < Utility::LEAST_HUMAN_GAP && it != foreCandidates.end()-1)
		{
			continueNum++;
			sumWeight += (*it)*foreHistByCol.at<double>(0,(*it))/1000;
			eachColSum += foreHistByCol.at<double>(0,(*it))/1000;
		}
		else
		{
			if(continueNum > Utility::HUMAN_WIDTH)
			{
				int personWidth = (*(it-1))-(*(it-continueNum));
				Mat person = Mat(doubleImg,Rect(*(it-continueNum),0,personWidth,doubleImg.rows));
				Mat personOnes = Mat::ones(doubleImg.rows,personWidth,CV_64F);
				Point2f center;
				center.x = sumWeight/eachColSum;
				double sumY = 0, sumWeightY = 0;
				for(int xPos=0; xPos<person.rows; xPos++)
					for(int yPos=0; yPos<person.cols; yPos++)
					{
						sumY += person.at<double>(xPos,yPos);
						sumWeightY += xPos*person.at<double>(xPos,yPos);
					}
					center.y = sumWeightY/sumY;
					double centerWeight = eachColSum/continueNum*1000;
					if(centerWeight > Utility::CENTER_WEIGHT_THRESH)
						baryCenters.push_back(center);
			}
			continueNum = 0;
			sumWeight = 0;
			eachColSum = 0;
		}
		lastIndex = (*it);
	}
	return 0;
}