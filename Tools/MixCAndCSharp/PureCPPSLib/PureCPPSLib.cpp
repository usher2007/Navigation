#include "PureCPPSLib.h"
using namespace cv;
MyImage::MyImage(const string& imageName)
{
	this->_img = imread(imageName);
}

void MyImage::ShowImage()
{
	namedWindow("lena", CV_WINDOW_AUTOSIZE);
	imshow("lena", _img);
	waitKey();
}