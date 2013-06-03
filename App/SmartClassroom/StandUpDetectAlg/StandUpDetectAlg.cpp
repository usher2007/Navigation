// StandUpDetectAlg.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "StandUpDetectAlg.h"
#include "Utility.h"
#include "StandUpAnalyzer.h"

double StandUpConfig::GBM_LEARNING_RATE		= 0.01;
int    StandUpConfig::FG_LOW_THRESH			= 128;
int    StandUpConfig::FG_UP_THRESH			= 255;
int    StandUpConfig::TRACK_INTERVAL		= 2;
int    StandUpConfig::DETECT_LINE			= 250;
int    StandUpConfig::ROW_NUM				= 9;
int    StandUpConfig::START_LEFT			= 100;
int    StandUpConfig::END_RIGHT				= 500;
int    StandUpConfig::CACHED_POS_COUNT		= 25;
int    StandUpConfig::MAX_GAP				= 50;
int    StandUpConfig::HUMAN_WIDTH			= 30;
int    StandUpConfig::MAX_HEIGHT_GAP		= 100;
int    StandUpConfig::HUMAN_HEIGHT			= 50;
int    StandUpConfig::CENTER_WEIGHT_THRESH  = 2000;
int    StandUpConfig::SLOPE_UP_THRESH       = -2.5;
int    StandUpConfig::SLOPE_DOWN_THRESH     = 2.5;

CStandUpDetectAlg::CStandUpDetectAlg(int index)
{
	cameraIndex = index;
	frameIndex = 0;
	cachedSums = new double[StandUpConfig::ROW_NUM];
	for(int i=0; i<StandUpConfig::ROW_NUM; ++i)
	{
		std::queue<int> singlePos;
		cachedSums[i] = 0;
		for(int j=0; j<StandUpConfig::CACHED_POS_COUNT; j++)
		{
			singlePos.push(StandUpConfig::DETECT_LINE);
			cachedSums[i] += StandUpConfig::DETECT_LINE;
		}
		cachedPosList.push_back(singlePos);
	}

	curStandUpRows.clear();
	m_pStandUpAnalyzer = CStandUpAnalyzer::GetInstance();
}

int CStandUpDetectAlg::Update(cv::Mat& frame)
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
	findStandUp();
	((CStandUpAnalyzer *)m_pStandUpAnalyzer)->AnalyzePosition(cameraIndex, curStandUpRowInfo);
}

int CStandUpDetectAlg::findStudentRanges()
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
		if(index - left < StandUpConfig::MAX_GAP)
		{
			right = index;
			continueCount++;
		}
		else
		{
			if(continueCount > StandUpConfig::HUMAN_WIDTH)
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

int CStandUpDetectAlg::findStandUp()
{
	curStandUpRowInfo.clear();
	for(int i=0; i<studentRanges.size(); ++i)
	{
		if(isStandUpOrSitDown(i))
		{
			// TODO: Add the pos to a data structure.
		}
	}

	return 0;
}

int CStandUpDetectAlg::isStandUpOrSitDown( int rangIdx )
{
	int width = studentRanges[rangIdx].right - studentRanges[rangIdx].left;
	int center = studentRanges[rangIdx].left + width / 2;
	int rowNum = (center - StandUpConfig::START_LEFT)/((StandUpConfig::END_RIGHT - StandUpConfig::START_LEFT)/StandUpConfig::ROW_NUM);
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
			if(rowIdx-prevIdx>StandUpConfig::MAX_HEIGHT_GAP)
			{
				if(countSum > StandUpConfig::HUMAN_HEIGHT)
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

	if(pos < StandUpConfig::DETECT_LINE && weight > StandUpConfig::CENTER_WEIGHT_THRESH)
	{
		cachedSums[rowNum] -= cachedPosList[rowNum].front();
		cachedPosList[rowNum].pop();
		cachedPosList[rowNum].push(pos);
		cachedSums[rowNum] += pos;
	}
	else
	{
		int avgPos = cachedSums[rowNum] / StandUpConfig::CACHED_POS_COUNT;
		cachedSums[rowNum] -= cachedPosList[rowNum].front();
		cachedPosList[rowNum].pop();
		cachedPosList[rowNum].push(avgPos);
		cachedSums[rowNum] += avgPos;
	}

	double slope = calcSlope(rowNum);
	if(slope < StandUpConfig::SLOPE_UP_THRESH)
	{
		curStandUpRows.insert(rowNum);
		StandUpInfo sInfo;
		sInfo.weight = weight;
		sInfo.pos = pos;
		sInfo.slope = slope;
		curStandUpRowInfo[rowNum] = sInfo;
		return 1;  // stand up
	}
	else if(slope > StandUpConfig::SLOPE_DOWN_THRESH)
	{
		if(curStandUpRows.find(rowNum) != curStandUpRows.end())
		{
			curStandUpRows.erase(rowNum);
		}
		StandUpInfo sInfo;
		sInfo.weight = weight;
		sInfo.pos = pos;
		sInfo.slope = slope;
		curStandUpRowInfo[rowNum] = sInfo;
		return -1; // sit down
	}
	return 0;
}

double CStandUpDetectAlg::calcSlope( int rowNum )
{
	static cv::Mat X = cv::Mat(1,StandUpConfig::CACHED_POS_COUNT, CV_64F);
	static cv::Mat Y = cv::Mat(1,StandUpConfig::CACHED_POS_COUNT, CV_64F); 
	std::queue<int> reverseQueue;
	for(int i=0; i<StandUpConfig::CACHED_POS_COUNT; ++i)
	{
		X.at<double>(0, i) = i+1;
		Y.at<double>(0, i) = cachedPosList[rowNum].front();
		int tmpPos = cachedPosList[rowNum].front();
		cachedPosList[rowNum].pop();
		reverseQueue.push(tmpPos);
	}

	for(int i=0; i<StandUpConfig::CACHED_POS_COUNT; ++i)
	{
		int tmpPos = reverseQueue.front();
		reverseQueue.pop();
		cachedPosList[rowNum].push(tmpPos);
	}

	double A = X.dot(X);
	double B = X.dot(cv::Mat::ones(1, StandUpConfig::CACHED_POS_COUNT, CV_64F));
	double C = X.dot(Y);
	double D = Y.dot(cv::Mat::ones(1, StandUpConfig::CACHED_POS_COUNT, CV_64F));
	return (C*StandUpConfig::CACHED_POS_COUNT - B*D) / (A*StandUpConfig::CACHED_POS_COUNT - B*B);
}

