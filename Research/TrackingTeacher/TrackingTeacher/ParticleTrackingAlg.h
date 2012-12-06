#pragma once
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ParticleFeature.h"
#include "Particle.h"
#include "Utility.h"
#include "TrackedPerson.h"
using namespace cv;

class ParticleTrackingAlg
{
public:
	ParticleTrackingAlg();
	int AddTrackedPerson(TrackedPerson person);
	int Tracking(const Mat& image);
	Rect GetPersonLocationById(const int id);
	bool PersonAlreadyTracked(const Point2f& center);
private:
	std::vector<TrackedPerson> personList;
	Mat frame;
	Mat image;
};
