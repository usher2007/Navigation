#include "opencv\cv.h"
#include "opencv\cxcore.h"
#include "opencv\highgui.h"
#include "opencv\cvaux.h"

class GaussBGM
{
public:
	GaussBGM(IplImage *pImg);
	~GaussBGM();
	void UpdateModel(IplImage *pImg);
	IplImage* GetForegroundImg();
	IplImage* GetBackgroundImg();
private:
	IplImage *pFrame;
	IplImage *pFrImg;
	IplImage *pBkImg;
	CvGaussBGModel *pBgModel;
};