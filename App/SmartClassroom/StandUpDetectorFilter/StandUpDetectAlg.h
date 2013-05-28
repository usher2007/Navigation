#pragma once
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <set>

typedef struct _stuRange
{
	int left;
	int right;
} StuRange;

typedef struct _standUpInfo
{
	double slope;
	double weight;
	int pos;
} StandUpInfo;

typedef std::vector<std::queue<int>> QueueList;

class StandUpDetectAlg
{
public:
	StandUpDetectAlg();
	int Update(cv::Mat& frame);

private:
	cv::BackgroundSubtractorMOG2 bgSubtractor;
	cv::Mat gbmForeground;
	cv::Mat doubleForeground;
	int frameIndex;
	std::vector<StuRange> studentRanges;
	QueueList cachedPosList;
	double *cachedSums;
	std::set<int> curStandUpRows;
	std::map<int, StandUpInfo> curStandUpRowInfo;

private:
	int findStudentRanges();
	int findStandUp();
	int isStandUpOrSitDown(int rangIdx);
	double calcSlope(int rowNum);
	
};