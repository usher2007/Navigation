#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "Utility.h"
#include "Person.h"
#include <iostream>
#include <string>
#include <time.h>

using namespace cv;
int CalcImageBaryCenters(const Mat& img, vector<Point2f>& baryCentres);

int main(int argc, char **argv)
{
	VideoCapture cap;
	Mat frame, gbmForeground;
	BackgroundSubtractorMOG2 bgSubtractor;
	cap.open(Utility::videoFileName);
	cap >> frame;
	namedWindow("Result");
	PersonManager personManager;
	long totalTime = 0, startTime = 0, endTime = 0;
	for(int index=0; ;index++)
	{
		if(frame.empty())
		{
			break;
		}
		if(index == 0)
		{
			bgSubtractor(frame, gbmForeground, 0.01);
			threshold(gbmForeground, gbmForeground, 128, 255, THRESH_BINARY);
			continue;
		}
		if(index%3 != 0)
		{
			cap >> frame;
			continue;
		}
		startTime = clock();
		bgSubtractor(frame, gbmForeground, 0.01);
		threshold(gbmForeground, gbmForeground, 128, 255, THRESH_BINARY);

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
		rectangle(frame, Utility::BeginTrackingArea, Scalar(255,0,0));
		rectangle(frame, Utility::StopTrackingArea, Scalar(0,0,255));
		personManager.DrawPersons(frame);
		imshow("Result", frame);
		endTime = clock();
		totalTime += endTime - startTime;
		waitKey(10);

		cap >> frame;
	}
	std::cout<<"TOTAL TIME:"<<totalTime/CLOCKS_PER_SEC<<std::endl;
	return 0;
}

int CalcImageBaryCenters(const Mat& img, vector<Point2f>& baryCentres)
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
		if(foreHistByCol.at<double>(0,j) > 2*avgForeHist)
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
		if((*it) - lastIndex < 50 && it != foreCandidates.end()-1)
		{
			continueNum++;
			sumWeight += (*it)*foreHistByCol.at<double>(0,(*it))/1000;
			eachColSum += foreHistByCol.at<double>(0,(*it))/1000;
		}
		else
		{
			if(continueNum > 30)
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
					if(centerWeight > 10)
						baryCentres.push_back(center);
			}
			continueNum = 0;
			sumWeight = 0;
			eachColSum = 0;
		}
		lastIndex = (*it);
	}
	return 0;
}