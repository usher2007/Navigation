#include "Utility.h"
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
		xMat = Mat(rows, cols, CV_8U);
		yMat = Mat(rows, cols, CV_8U);
		allOneMat = Mat::ones(rows, cols, CV_8U);
		if(xMat.rows != yMat.rows || xMat.cols != yMat.cols)
		{
			return -1;
		}
		for(int i=0; i<rows; i++)
			for(int j=0; j<cols; j++)
			{
				xMat.at<uchar>(i,j) = j;
				yMat.at<uchar>(i,j) = i;
			}

		return 0;
	}

	int CalcImageBaryCentre(const Mat& img, const int noneZeroPointsNum, Point2f& baryCentre)
	{
		baryCentre.x = xMat.dot(img)/noneZeroPointsNum;
		baryCentre.y = yMat.dot(img)/noneZeroPointsNum;
		return 0;
	}
}