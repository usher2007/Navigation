#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <iostream>
#include <string>
#include <time.h>

using namespace cv;

int main(int argc, char **argv)
{
	const string VIDEO_FILE_NAME = "H:\\GitHubCode\\Navigation\\Research\\TrackingTeacher\\Data\\S1_STANDUP1.mp4";
	const string RESULT_FILE_NAME = "H:\\GitHubCode\\Navigation\\Research\\TrackingByGBM\\Data\\12.12\\S1_STANDUP1.avi";
	VideoCapture cap;
	Mat lastFrame, frame, gbmForeground, diff;
	BackgroundSubtractorMOG2 bgSubtractor;
	cap.open(VIDEO_FILE_NAME);
	cap >> frame;
	lastFrame = frame;
	cap >>frame;
	namedWindow("Result");
	VideoWriter videoWriter(RESULT_FILE_NAME, CV_FOURCC('X', 'V', 'I', 'D'), 25, frame.size());

	for(int index=0; ;index++)
	{
		if(frame.empty())
		{
			break;
		}
		diff = abs(frame - lastFrame);

		bgSubtractor(frame, gbmForeground, 0.05);
		threshold(gbmForeground, gbmForeground, 128, 255, THRESH_BINARY);

		imshow("Result", diff/255);
		char *fileName = new char[1024];
		sprintf(fileName, "H:\\GitHubCode\\Navigation\\Research\\LocateStudent\\Data\\12.19\\TT%d.bmp", index);
		imwrite(fileName, frame);
		waitKey(4);
		lastFrame = frame;
		cap >> frame;
	}

	return 0;
}