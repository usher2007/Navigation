#pragma once
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
using namespace cv;

class StandUpConfig
{
public:
	static string VIDEO_FILE_NAME;
	static string RESULT_FILE_NAME;
	static cv::Rect BEGIN_TRACKING_AREA;
	static cv::Rect STOP_TRACKING_AREA;
	static int LEAST_HUMAN_GAP;
	static int HUMAN_WIDTH;
	static int DISAPPEAR_FRAME_THRESH;
	static int CENTER_WEIGHT_THRESH;
	static int FG_LOW_THRESH;
	static int FG_UP_THRESH;
	static double FG_HIST_THRESH;
	static double GBM_LEARNING_RATE;
	static int TRACK_INTERVAL;
	static double PI;
	static int DETECT_LINE;
	//For Debug
	static cv::Rect PAD_AREA_1;
	static cv::Rect PAD_AREA_2;
	static cv::Rect PAD_AREA_3;
	static cv::Rect PAD_AREA_4;
	static cv::Rect PAD_AREA_5;
	static cv::Rect PAD_AREA_6;
};