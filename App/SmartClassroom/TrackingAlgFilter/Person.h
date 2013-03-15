#pragma once
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"

class Person
{
public:
	Person(const cv::Point2f& initialCenter);
	cv::Point2f GetBaryCenter();
	int GetCenterDistance(const cv::Point2f& newCenter);
	int UpdateBaryCenter(const cv::Point2f& newCenter);
	bool Disappeared();
	bool IsNoise();
	int SetCurrentStatus(bool detectedOrTracked);
	bool IsDetectedOrTracked();
	int UpdateDetectAndDisappearTimes();
private:
	bool detectedOrTracked;
	cv::Point2f baryCenter;
	int detectedTimes;
	int disappearTimes;
};

class PersonManager
{
public:
	PersonManager();
	int ResetAllPersonStatus();
	int ProcessBaryCenter(const cv::Point2f& baryCenter);
	int Update();
	int DrawPersons(cv::Mat& image);
	std::vector<cv::Point2f> GetTrackedPersons();
private:
	std::vector<Person> detectedPersons;
	std::vector<Person> trackedPersons;
private:
	int updateDetectedPersons();
	int updateTrackedPersons();
};