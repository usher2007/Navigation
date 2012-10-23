#pragma once
#include "ParticleFeature.h"

class RGBHistFeature : public ParticleFeature
{
public:
	RGBHistFeature(){}
	RGBHistFeature(const int rBins = 6, const int gBins = 6, const int bBins = 6 , const float rMin = 0, const float rMax = 256, const float gMin = 0, const float gMax = 256, const float bMin = 0, const float bMax = 256, const double noiseWeight = 0.05);
	int SetRGBBins(const int rBins, const int gBins, const int bBins);
	int SetRGBRanges(const float rMin, const float rMax, const float gMin, const float gMax, const float bMin, const float bMax);
	int SetNoiseWeight(const double noiseWeight);
	//From Interface ParticleFeature
	double  CalcSimilarityToOrigin(const Mat& particleRoi);
	int GenerateOriginFeature(const Mat& roi);
private:
	int histSize[3];
	float (*pRanges)[2];
	double noiseWeight;
	Mat originHist;

	const Mat& generateHist(const Mat& roi);
};