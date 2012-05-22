#include "opencv\cv.h"
#include "opencv\highgui.h"
#include "DepthMapSkt.h"
#include "DepthMapSktBinFileIO.h"
#include "GaussBGM.h"
#include <string>
#include <iostream>
using std::string;
using std::to_string;

int main(int argc, char** argv)
{
	void saveImage(int, IplImage*, IplImage*,IplImage*,IplImage*);
	IplImage* pScaledDepthImage;
	CvCapture* pCapture = NULL;
	IplImage* pFrame = NULL;
	IplImage* pBkImg = NULL;
	IplImage* pFrImg = NULL;
	IplImage* pFrResImg = NULL;
	IplImage* pMask = NULL;
	char videoFileName[] = "D:\\code\\Data\\cheer up\\a08_s01_e02_rgb.avi";
	char depthFileName[] = "D:\\code\\Data\\cheer up\\a08_s01_e02_depth.bin";
	int nofs = 0; //number of frames conatined in the file (each file is a video sequence of depth maps)
	int ncols = 0;
	int nrows = 0;
	FILE * fp = fopen(depthFileName, "rb");

	if(fp == NULL)
	{
		fprintf(stderr, "Can't open video file %s\n", depthFileName);
		return -1;
	}
	if(!(pCapture = cvCaptureFromFile(videoFileName)))
	{
		fprintf(stderr, "Can't open video file %s\n", videoFileName);
		return -1;
	}
	
	ReadDepthMapSktBinFileHeader(fp, nofs, ncols, nrows);

	printf("number of frames=%i\n", nofs);

	cvNamedWindow("Depth Image", 1);
	cvNamedWindow("Foreground", 1);
	cvNamedWindow("Segment Result", 1);
	cvMoveWindow("Depth Image", 0, 0);
	cvMoveWindow("Foreground", 0, 640);
	cvMoveWindow("Segment Result", 480, 0);
	
	GaussBGM *bgModel= NULL;
	int frameCount = 0;
	while(pFrame = cvQueryFrame(pCapture))
	{
		frameCount ++;
		CDepthMapSkt depthMap;
		depthMap.SetSize(ncols, nrows); //it allocates space
		//the data will be stored in <depthMap>
		ReadDepthMapSktBinFileNextFrame(fp, ncols, nrows, depthMap);

		if(frameCount == 1)
		{
			bgModel = new GaussBGM(pFrame);
			pMask = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 1);
			pFrResImg = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 3);
		}
		else
		{
			float scaleWidthCoeff = pFrame->width/(float)ncols;
			float scaleHeightCoeff = pFrame->height/(float)nrows;
			depthMap.ScaleSizeNN(scaleWidthCoeff, scaleHeightCoeff);
			depthMap.ShiftNN(10,30);

			CvSize scaledDepthImgSize;
			scaledDepthImgSize.width = pFrame->width;
			scaledDepthImgSize.height = pFrame->height;
			pScaledDepthImage = cvCreateImage(scaledDepthImgSize,IPL_DEPTH_8U,1);
			depthMap.convertToChar((uchar *)pScaledDepthImage->imageData);
			pMask->imageData = (char *)depthMap.GetForegroundMask();
			memset(pFrResImg->imageData, 0, pFrame->imageSize);
			cvCopy(pFrame, pFrResImg, pMask);
			bgModel->UpdateModel(pFrame);
			pBkImg = bgModel->GetBackgroundImg();
			pFrImg = bgModel->GetForegroundImg();

			/*cvShowImage("Depth Image", pScaledDepthImage);
			cvShowImage("Foreground", pFrImg);
			cvShowImage("Segment Result", pFrResImg);*/
			saveImage(frameCount, pFrame, pScaledDepthImage, pFrImg, pFrResImg);

			cvWaitKey(2);
			cvReleaseImage(&pScaledDepthImage);
		}

		
	}

	fclose(fp);
	fp=NULL;

	return 0;
}
string resultFolder = "D:\\Navigation\\Research\\Kinect\\SegmentKinectVideos\\Data\\";
string originalFolder = "Original\\";
string depthFolder = "DepthImage\\";
string foregroundFolder = "GBMForeground\\";
string segmentFolder = "SegmentResult\\";
void saveImage(int index, IplImage *pOriginal, IplImage *pDepthImage, IplImage *pForegroundImage, IplImage *pSegmentResult)
{
	std::stringstream ssIndex;
	ssIndex << index;
	string s = resultFolder + originalFolder + ssIndex.str() + ".bmp";
	const char *fileName = s.c_str();
	cvSaveImage(fileName, pOriginal);
	s = resultFolder + depthFolder + ssIndex.str() + ".bmp";
	fileName = s.c_str();
	cvSaveImage(fileName, pDepthImage);
	s = resultFolder + foregroundFolder + ssIndex.str() + ".bmp";
	fileName = s.c_str();
	cvSaveImage(fileName, pForegroundImage);
	s = resultFolder + segmentFolder + ssIndex.str() + ".bmp";
	fileName = s.c_str();
	cvSaveImage(fileName, pSegmentResult);
}