#include "stdafx.h"
#include "StandUpDetectAlg.h"
#include "Utility.h"

double StandUpConfig::GBM_LEARNING_RATE = 0.01;
int    StandUpConfig::FG_LOW_THRESH     = 128;
int    StandUpConfig::FG_UP_THRESH      = 255;
int    StandUpConfig::TRACK_INTERVAL    = 2;
int    StandUpConfig::DETECT_LINE       = 250;

StandUpDetectAlg::StandUpDetectAlg()
{
	frameIndex = 0;
	cachedPosSum = 0;
	for(int i=0; i<25; i++)
	{
		cachedPos.push(250);
		cachedPosSum += 250;
	}
}

int StandUpDetectAlg::Update(cv::Mat& frame)
{
	if(frame.empty())
	{
		return -1;
	}

	if(frameIndex == 0)
	{
		bgSubtractor(frame, gbmForeground, StandUpConfig::GBM_LEARNING_RATE);
		cv::threshold(gbmForeground, gbmForeground, StandUpConfig::FG_LOW_THRESH, StandUpConfig::FG_UP_THRESH, THRESH_BINARY);
		frameIndex ++;
		return 0;
	}

	if(frameIndex%StandUpConfig::TRACK_INTERVAL != 0)
	{
		frameIndex++;
		return 0;
	}

	bgSubtractor(frame, gbmForeground, StandUpConfig::GBM_LEARNING_RATE);
	cv::threshold(gbmForeground, gbmForeground, StandUpConfig::FG_LOW_THRESH, StandUpConfig::FG_UP_THRESH, THRESH_BINARY);

	gbmForeground = gbmForeground/255;
	findStudentRanges();
}

int StandUpDetectAlg::findStudentRanges()
{
	studentRanges.clear();
	doubleForeground= cv::Mat(gbmForeground.rows, gbmForeground.cols, CV_64F);
	gbmForeground.convertTo(doubleForeground, CV_64F);

	static cv::Mat leftOnes = cv::Mat::ones(1, gbmForeground.rows, CV_64F);
	static cv::Mat rightOnes = cv::Mat::ones(gbmForeground.cols, 1, CV_64F);
	cv::Mat foreHistByCol = leftOnes*doubleForeground;
	cv::Mat tmpRes = foreHistByCol*rightOnes;
	double avgForeHist = tmpRes.at<double>(0,0)/foreHistByCol.cols;
	int maxCol = foreHistByCol.cols;

	std::vector<int> candidates;
	for(int j=0; j<maxCol; j++)
	{
		if(foreHistByCol.at<double>(0,j) > 3*avgForeHist)
		{
			candidates.push_back(j);
		}
	}

	int left = 0, right = 0, continueCount = 0;;
	int index = 0;
	StuRange range;
	while(index < candidates.size())
	{
		if(index - left < 50)
		{
			right = index;
			continueCount++;
		}
		else
		{
			if(continueCount > 30)
			{
				range.left = candidates[left];
				range.right = candidates[right];
				studentRanges.push_back(range);
			}
			left = index;
			continueCount = 0;
		}
	}

	return 0;
}

int StandUpDetectAlg::findStandUp()
{
	for(int i=0; i<studentRanges.size(); ++i)
	{
		if(isStandUp(i))
		{
			// TODO: Add the pos to a data structure.
		}
	}

	return 0;
}

bool StandUpDetectAlg::isStandUp(int rangIdx)
{
	int width = studentRanges[rangIdx].right - studentRanges[rangIdx].left;
	cv::Rect roi(0,studentRanges[rangIdx].left, width, gbmForeground.rows);
	cv::Mat curCandidate = doubleForeground(roi);

	width = curCandidate.cols;
	int height = curCandidate.rows;
	cv::Mat rightOnes = cv::Mat::ones(width, 1, CV_64F);
	cv::Mat leftOnes = cv::Mat::ones(1,height, CV_64F);

	cv::Mat histByRow = curCandidate*rightOnes;
	cv::Mat tempSum = leftOnes*histByRow;
	double avgByRow = tempSum.at<double>(0,0)/height;

	int prevIdx = 0;
	int countSum = 0;
	std::vector<int> pixIdx;
	int weightSum = 0;
	int posSum = 0;
	int totalSum = 0;
	for(int rowIdx=0; rowIdx<height; ++rowIdx)
	{
		if(histByRow.at<double>(rowIdx,0) > 3*avgByRow)
		{
			if(rowIdx-prevIdx>100)
			{
				if(countSum > 50)
				{
					for(int j=0; j<countSum; j++)
					{
						weightSum += histByRow.at<double>(pixIdx[j], 0);
						totalSum += histByRow.at<double>(pixIdx[j], 0) * pixIdx[j];
					}
					posSum += countSum;
				}
				countSum = 0;
				pixIdx.clear();
			}
			else
			{
				countSum++;
				pixIdx.push_back(rowIdx);
			}
		}
	}

	if(posSum == 0 || weightSum == 0)
	{
		return false;
	}

	int pos = 0;
	double weight = 0.0;
	pos = totalSum / weightSum;
	weight = weightSum * 1.0 / posSum;

	if(pos < 250 && weight > 2000)
	{
		cachedPosSum -= cachedPos.front();
		cachedPos.pop();
		cachedPos.push(pos);
		cachedPosSum += pos;
	}
	else
	{
		int avgPos = cachedPosSum / 25;
		cachedPosSum -= cachedPos.front();
		cachedPos.pop();
		cachedPos.push(avgPos);
		cachedPosSum += avgPos;
	}

	double slope = calcSlope();
	if(slope < -2.5)
	{
		return true;
	}
	return false;
}

double StandUpDetectAlg::calcSlope()
{
	static cv::Mat X = cv::Mat(1,25, CV_64F);
	static cv::Mat Y = cv::Mat(1,25, CV_64F); 
	std::queue<int> reverseQueue;
	for(int i=0; i<25; ++i)
	{
		X.at<double>(0, i) = i+1;
		Y.at<double>(0, i) = cachedPos.front();
		int tmpPos = cachedPos.front();
		cachedPos.pop();
		reverseQueue.push(tmpPos);
	}

	for(int i=0; i<25; ++i)
	{
		int tmpPos = reverseQueue.front();
		reverseQueue.pop();
		cachedPos.push(tmpPos);
	}

	double A = X.dot(X);
	double B = X.dot(cv::Mat::ones(1, 25, CV_64F));
	double C = X.dot(Y);
	double D = Y.dot(cv::Mat::ones(1, 25, CV_64F));
	return (C*25 - B*D) / (A*25 - B*B);
}
