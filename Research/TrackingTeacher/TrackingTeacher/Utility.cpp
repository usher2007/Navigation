#include "Utility.h"
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
namespace Utility
{
	Mat allOneMat;
	Mat xMat;
	Mat yMat;
	int GetNoneZeroPointsNum(const Mat& img)
	{
		int result =  img.dot(allOneMat);
		return result>0 ? result : -1;
	}

	int GenerateAuxMatForBaryCentre(int rows, int cols)
	{
		xMat = Mat(rows, cols, CV_64F);
		yMat = Mat(rows, cols, CV_64F);
		allOneMat = Mat::ones(rows, cols, CV_8U);
		for(int i=0; i<rows; i++)
			for(int j=0; j<cols; j++)
			{
				xMat.at<double>(i,j) = j;
				yMat.at<double>(i,j) = i;
			}

		return 0;
	}

	int CalcImageBaryCentre(const Mat& img, const int noneZeroPointsNum, Point2f& baryCentre)
	{
		Mat doubleImg = Mat(img.rows, img.cols, CV_64F);
		img.convertTo(doubleImg, CV_64F);
		baryCentre.x = xMat.dot(doubleImg)/noneZeroPointsNum;
		baryCentre.y = yMat.dot(doubleImg)/noneZeroPointsNum;
		return 0;
	}
}