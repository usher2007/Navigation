#include "cv.h"
#include "highgui.h"

using namespace cv;
int main( int argc, char* argv[])
{
	Mat img = imread("C:\\Users\\usher\\Desktop\\pic\\ou.jpg");

	if (!img.data)
	{
		return -1;
	}

	namedWindow("loveLena", CV_WINDOW_AUTOSIZE);

	imshow("loveLena", img);

	waitKey();

	return 0;
}