#include "opencv\cv.h"
#include "opencv\highgui.h"
#include "DepthMapSkt.h"
#include "DepthMapSktBinFileIO.h"
#include "GaussBGM.h"

int main(int argc, char** argv)
{
	IplImage* pImage;
	char depthFileName[] = "D:\\code\\Data\\cheer up\\a08_s01_e02_depth.bin";
	FILE * fp = fopen(depthFileName, "rb");
	if(fp == NULL)
	{
		fprintf(stderr, "Can't open video file %s\n", depthFileName);
		return -1;
	}

	CvCapture* pCapture = NULL;
	IplImage* pFrame = NULL;
	IplImage* pBkImg = NULL;
	IplImage* pFrImg = NULL;
	IplImage* pFrResImg = NULL;
	IplImage* pMask = NULL;
	char videoFileName[] = "D:\\code\\Data\\cheer up\\a08_s01_e02_rgb.avi";
	if(!(pCapture = cvCaptureFromFile(videoFileName)))
	{
		fprintf(stderr, "Can't open video file %s\n", videoFileName);
		return -1;
	}
	int nofs = 0; //number of frames conatined in the file (each file is a video sequence of depth maps)
	int ncols = 0;
	int nrows = 0;
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
			float scaleWidth = pFrame->width/(float)ncols;
			float scaleHeight = pFrame->height/(float)nrows;
			depthMap.ScaleSizeNN(scaleWidth, scaleHeight);

			CvSize mImgSize;
			mImgSize.width = pFrame->width;
			mImgSize.height = pFrame->height;
			pImage = cvCreateImage(mImgSize,IPL_DEPTH_8U,1);
			depthMap.convertToChar((uchar *)pImage->imageData);
			cvShowImage("Depth Image", pImage);
			pMask->imageData = (char *)depthMap.GetForegroundMask();
			memset(pFrResImg->imageData, 0, pFrame->imageSize);
			cvCopy(pFrame, pFrResImg, pMask);
			bgModel->UpdateModel(pFrame);
			pBkImg = bgModel->GetBackgroundImg();
			pFrImg = bgModel->GetForegroundImg();
			cvShowImage("Foreground", pFrImg);
			cvShowImage("Segment Result", pFrResImg);

			cvWaitKey(2);
			cvReleaseImage(&pImage);
		}

		
	}

	fclose(fp);
	fp=NULL;

	return 0;
}