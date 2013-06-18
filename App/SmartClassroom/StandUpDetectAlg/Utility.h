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
	static int FG_LOW_THRESH;
	static int FG_UP_THRESH;
	static double FG_HIST_THRESH;
	static double GBM_LEARNING_RATE;
	static int TRACK_INTERVAL;
	static double PI;
	static int DETECT_LINE;
	static int START_LEFT;
	static int END_RIGHT;
	static int ROW_NUM;
	static int CACHED_POS_COUNT;
	static int CACHED_SLOPE_COUNT;
	static int MAX_GAP;
	static int HUMAN_WIDTH;
	static int HUMAN_HEIGHT;
	static int MAX_HEIGHT_GAP;
	static int CENTER_WEIGHT_THRESH;
	static double SLOPE_UP_THRESH;
	static double SLOPE_DOWN_THRESH;
};