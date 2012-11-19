#pragma once
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
using namespace cv;
namespace Utility
{
	const double PI = 3.1415926;
	const int xNoise = 5;
	const int yNoise = 2;
	const int particleNum = 50;
	const int rBins = 6;
	const int gBins = 6;
	const int bBins = 6;
	const int rMin = 0;
	const int rMax = 256;
	const int gMin = 0;
	const int gMax = 256;
	const int bMin = 0;
	const int bMax = 256;
	extern Mat allOneMat;
	extern Mat xMat;
	extern Mat yMat;
	int GetNoneZeroPointsNum(const Mat& img);
	int CalcImageBaryCentre(const Mat& img, const int noneZeroPointsNum, Point2f& baryCentre);
	int GenerateAuxMatForBaryCentre(int rows, int cols);
}