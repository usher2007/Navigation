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
	cv::Mat rightOnes = cv::Mat::ones(width, 1, CV_64F);
	cv::Mat curCandidate = cv::Mat(gbmForeground.rows, width, CV_64F);
	doubleForeground.adjustROI(0, gbmForeground.rows-1, studentRanges[rangIdx].left, studentRanges[rangIdx].right);
	doubleForeground.copyTo(curCandidate);
}