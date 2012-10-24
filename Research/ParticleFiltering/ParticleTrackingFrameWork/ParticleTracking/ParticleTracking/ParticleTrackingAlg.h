#pragma once
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ParticleFeature.h"
#include "Particle.h"
#include "Utility.h"
using namespace cv;

class ParticleTrackingAlg
{
public:
	ParticleTrackingAlg(int particleNum);
	int AddParticleFeature(ParticleFeature* feature);
	int SetInitialObject(const Rect& trackObject, const Mat& image);
	int Tracking(VideoCapture cap);
private:
	std::vector<ParticleFeature *> features;
	std::vector<Particle> particleList;
	Rect trackObject;
	Rect resultObject;
	int particleNum;
	Mat frame;
	Mat image;
private:
	int generateParticleList();
	int resampleParticleList();
};
