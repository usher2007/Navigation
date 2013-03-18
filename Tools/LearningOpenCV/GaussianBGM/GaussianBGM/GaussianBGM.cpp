#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
using namespace cv;

const string videoFileName = "C:\\Users\\doba\\Desktop\\Downloads\\T_WALK1.mp4";
int main(void)
{
	VideoCapture cap;
	bool res = cap.open(videoFileName);
	Mat frame, foreground;
	BackgroundSubtractorMOG2 mog;
	mog.backgroundRatio = 0.7;
	mog.history = 200;
	//mog.varThreshold = 6.25;
	namedWindow("Original");
	namedWindow("Results1");
	namedWindow("Results2");
	for(;;)
	{
		cap >> frame;
		if(frame.empty())
		{
			break;
		}
		mog(frame, foreground, 0.01);
		imshow("Original", frame);
		imshow("Result1", foreground);
		threshold(foreground, foreground, 128, 255, THRESH_BINARY_INV);
		imshow("Result2", foreground);
		waitKey(10);
	}
}