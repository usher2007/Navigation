#pragma once
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "Utility.h"
#include "Person.h"
#include <iostream>
#include <string>
#include <time.h>
#include <streams.h>

typedef std::vector<cv::Point2f> cvBZoneVertexList;
typedef std::vector<cv::Point2f>::iterator cvBZoneVertexIter;
typedef std::vector<cv::Mat> cvBlindZoneList;
typedef std::vector<cv::Mat>::iterator cvBlindZoneIter;

typedef struct BlindZoneShape
{
	int x[4];
	int y[4];
} BlindZoneShape;
typedef std::vector<BlindZoneShape> BZoneShapeList;
typedef std::vector<BlindZoneShape>::iterator BZoneShapeIter;

class TrackingAlg
{
public:
	TrackingAlg();
	int Update(cv::Mat& frame);
	std::vector<cv::Point2f> GetTrackedPerson();
	HRESULT SetShowTracking(BOOL bShowTracking);
	HRESULT CacheAndShowBZoneVertex(int xPix, int yPix);
	HRESULT EraseCachedVertexes();
	HRESULT AddBZone(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	HRESULT DrawBZoneVertexes(cv::Mat &frame);
	HRESULT DrawBZones(cv::Mat &frame);
private:
	cv::BackgroundSubtractorMOG2 bgSubtractor;
	PersonManager personManager;
	int frameIndex;
	cv::Mat gbmForeground;
	BOOL bShowTrackingResult;

	cvBZoneVertexList m_cachedBZoneVertexes;
	cvBlindZoneList m_blindZoneList;
	BZoneShapeList m_bZoneShapeList;
	CCritSec m_vertexMutex;
	CCritSec m_bzoneMutex;

private:
	int CalcImageBaryCenters(const cv::Mat& img, std::vector<cv::Point2f>& baryCenters);
	bool containSameBZoneShape(const BlindZoneShape& bZoneShape);

	int drawAuxInfo(cv::Mat& frame);
	int drawBZoneVertexes(cv::Mat &frame);
	int drawBZones(cv::Mat &frame);
};