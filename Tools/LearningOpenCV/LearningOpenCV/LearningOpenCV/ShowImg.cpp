#include "opencv\cv.h"
#include "opencv\highgui.h"

using namespace cv;
int main( int argc, char* argv[])
{
	Mat img = imread("E:\\4341055.bmp");

	if (!img.data)
	{
		return -1;
	}

	namedWindow("loveLena", CV_WINDOW_AUTOSIZE);

	imshow("loveLena", img);

	waitKey();

	return 0;
}