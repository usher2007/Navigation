#include "GaussBGM.h"

GaussBGM::GaussBGM(IplImage *pImg)
{
	pFrame = pImg;
	pFrImg = cvCreateImage(cvSize(pFrame->width,pFrame->height), IPL_DEPTH_8U, 1);
	pBkImg = cvCreateImage(cvSize(pFrame->width,pFrame->height), IPL_DEPTH_8U, 3);
	pBgModel = (CvGaussBGModel *)cvCreateGaussianBGModel(pFrame, 0);
}

GaussBGM::~GaussBGM()
{
	cvReleaseImage(&pFrImg);
	cvReleaseImage(&pBkImg);
	cvReleaseBGStatModel((CvBGStatModel **)&pBgModel);
}

void GaussBGM::UpdateModel(IplImage *pImg)
{
	pFrame = pImg;
	cvUpdateBGStatModel(pFrame,(CvBGStatModel *)pBgModel);
}

IplImage* GaussBGM::GetBackgroundImg()
{
	cvCopy(pBgModel->background, pBkImg, 0);
	pBkImg->origin = 0;
	return pBkImg;
}

IplImage* GaussBGM::GetForegroundImg()
{
	cvCopy(pBgModel->foreground, pFrImg, 0);
	pFrImg->origin = 0;
	return pFrImg;
}