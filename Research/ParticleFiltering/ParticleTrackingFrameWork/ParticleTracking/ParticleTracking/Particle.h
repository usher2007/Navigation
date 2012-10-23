#pragma once
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
class Particle
{
public:
	Particle(const int xPos, const int yPos, const int width, const int height);
	Mat GetParticleRoi(const Mat& image);
	double GetParticleWeight();
	int SetParticleWeight(double weight);
private:
	Rect particleRegion;
	double weight;
};