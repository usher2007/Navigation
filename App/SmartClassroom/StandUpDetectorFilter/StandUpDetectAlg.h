#pragma once
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <iostream>
#include <vector>
#include <queue>

typedef struct _stuRange
{
	int left;
	int right;
} StuRange;

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
	std::queue<int> cachedPos;
	double cachedPosSum;

private:
	int findStudentRanges();
	int findStandUp();
	bool isStandUp(int rangIdx);
	double calcSlope();
	
};