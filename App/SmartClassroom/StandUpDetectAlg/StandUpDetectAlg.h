// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the STANDUPDETECTALG_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// STANDUPDETECTALG_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef STANDUPDETECTALG_EXPORTS
#define STANDUPDETECTALG_API __declspec(dllexport)
#else
#define STANDUPDETECTALG_API __declspec(dllimport)
#endif
#include <iostream>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include "DataStruct.h"

typedef struct _stuRange
{
	int left;
	int right;
} StuRange;



typedef std::vector<std::queue<int>> QueueList;


// This class is exported from the StandUpDetectAlg.dll
class STANDUPDETECTALG_API CStandUpDetectAlg {
public:
	CStandUpDetectAlg(int);
	int Update(cv::Mat& frame);

private:
	cv::BackgroundSubtractorMOG2 bgSubtractor;
	cv::Mat gbmForeground;
	cv::Mat doubleForeground;
	int frameIndex;
	std::vector<StuRange> studentRanges;
	QueueList cachedPosList;
	double *cachedSums;
	std::set<int> curStandUpRows;
	StandUpRowInfo curStandUpRowInfo;
	int cameraIndex;

	void *m_pStandUpAnalyzer;

private:
	int findStudentRanges();
	int mergeStudentRanges();
	int findStandUp();
	int handleStandUpOrSitDownPerRow(int rangIdx);
	double calcSlope(int rowNum);
};

