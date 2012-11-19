#pragma once
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

class ParticleFeature
{
public:
	ParticleFeature(double weight):featureWeight(weight){}
	virtual double GetFeatureWeight(){return featureWeight;}
	virtual double CalcSimilarityToOrigin(const Mat& particleRoi) = 0;
	virtual int GenerateOriginFeature(const Mat& roi) = 0;
	virtual ~ParticleFeature(){}
private:
	double featureWeight;
};

