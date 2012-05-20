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
	cvNamedWindow("Video", 1);
	cvNamedWindow("Background", 1);
	cvNamedWindow("Foreground", 1);
	cvMoveWindow("Depth Image", 30, 0);
	cvMoveWindow("Video", 30, 360);
	cvMoveWindow("Background", 360, 0);
	cvMoveWindow("Foreground", 360, 360);
	
	GaussBGM *bgModel= NULL;
	while(pFrame = cvQueryFrame(pCapture))
	{
		CDepthMapSkt depthMap;
		depthMap.SetSize(ncols, nrows); //it allocates space
		//the data will be stored in <depthMap>
		ReadDepthMapSktBinFileNextFrame(fp, ncols, nrows, depthMap);
		CvSize mImgSize;
		mImgSize.width = ncols;
		mImgSize.height = nrows;
		pImage = cvCreateImage(mImgSize,IPL_DEPTH_8U,1);
		depthMap.convertToChar((uchar *)pImage->imageData);
		cvShowImage("Depth Image", pImage);

		if(bgModel == NULL)
		{
			bgModel = new GaussBGM(pFrame);
		}
		else
		{
			bgModel->UpdateModel(pFrame);
			pBkImg = bgModel->GetBackgroundImg();
			pFrImg = bgModel->GetForegroundImg();
			cvShowImage("Video", pFrame);
			cvShowImage("Background", pBkImg);
			cvShowImage("Foreground", pFrImg);
		}

		cvWaitKey(2);
		cvReleaseImage(&pImage);
	}

	fclose(fp);
	fp=NULL;

	return 0;
}