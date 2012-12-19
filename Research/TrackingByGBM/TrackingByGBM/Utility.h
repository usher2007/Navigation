#pragma once
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
using namespace cv;

namespace Utility
{
	const string VIDEO_FILE_NAME = "H:\\GitHubCode\\Navigation\\Research\\TrackingTeacher\\Data\\T_WALK4.mp4";
	const string RESULT_FILE_NAME = "H:\\GitHubCode\\Navigation\\Research\\TrackingByGBM\\Data\\12.12\\T_WALK4.avi";
	const double PI = 3.1415926;
	const Rect BEGIN_TRACKING_AREA(120, 115, 480, 346);
	const Rect STOP_TRACKING_AREA(90, 72, 540, 432);
	const int DISAPPEAR_FRAME_THRESH = 10;
	const int LEAST_HUMAN_GAP = 50;
	const int HUMAN_WIDTH = 30;
	const int CENTER_WEIGHT_THRESH = 10;
	const int TRACK_INTERVAL = 1;
	const int FG_LOW_THRESH = 128;
	const int FG_UP_THRESH = 255;
	const double GBM_LEARNING_RATE = 0.01;
	const double FG_HIST_THRESH = 2;
}