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

	int CalcImageBaryCentre(const Mat& img, vector<Point2f>& baryCentres)
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
}