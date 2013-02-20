#pragma once
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"

using namespace cv;

class Person
{
public:
	Person(const Point2f& initialCenter);
	Point2f GetBaryCenter();
	int GetCenterDistance(const Point2f& newCenter);
	int UpdateBaryCenter(const Point2f& newCenter);
	bool Disappeared();
	bool IsNoise();
	int SetCurrentStatus(bool detectedOrTracked);
	bool IsDetectedOrTracked();
	int UpdateDetectAndDisappearTimes();
private:
	bool detectedOrTracked;
	Point2f baryCenter;
	int detectedTimes;
	int disappearTimes;
};

class PersonManager
{
public:
	PersonManager();
	int ResetAllPersonStatus();
	int ProcessBaryCenter(const Point2f& baryCenter);
	int Update();
	int DrawPersons(Mat& image);
	vector<Point2f> GetTrackedPersons();
private:
	vector<Person> detectedPersons;
	vector<Person> trackedPersons;
private:
	int updateDetectedPersons();
	int updateTrackedPersons();
};