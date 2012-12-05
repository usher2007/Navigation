#include "RGBHistFeature.h"
#include "Utility.h"
#include <time.h>
#include <iostream>

RGBHistFeature::RGBHistFeature(const int rBins /* = 6 */, const int gBins /* = 6 */, const int bBins /* = 6  */, const float rMin /* = 0 */, const float rMax /* = 256 */, const float gMin /* = 0 */, const float gMax /* = 256 */, const float bMin /* = 0 */, const float bMax /* = 256 */, const double noiseWeight /* = 0.05 */, const double featureWeight /* = 1.0 */)
	: ParticleFeature(featureWeight)
{
	histSize[0] = rBins;
	histSize[1] = gBins;
	histSize[2] = bBins;
	pRanges[0][0] = rMin;
	pRanges[0][1] = rMax;
	pRanges[1][0] = gMin;
	pRanges[1][1] = gMax;
	pRanges[2][0] = bMin;
	pRanges[2][1] = bMax;
	this->noiseWeight = noiseWeight;
}

inline int RGBHistFeature::SetRGBBins(const int rBins, const int gBins, const int bBins)
{
	histSize[0] = rBins;
	histSize[1] = gBins;
	histSize[2] = bBins;
	return 0;
}

inline int RGBHistFeature::SetRGBRanges(const float rMin, const float rMax, const float gMin, const float gMax, const float bMin, const float bMax)
{
	pRanges[0][0] = rMin;
	pRanges[0][1] = rMax;
	pRanges[1][0] = gMin;
	pRanges[1][1] = gMax;
	pRanges[2][0] = bMin;
	pRanges[2][1] = bMax;
	return 0;
}

inline int RGBHistFeature::SetNoiseWeight(const double noiseWeight)
{
	this->noiseWeight = noiseWeight;
	return 0;
}

double RGBHistFeature::CalcSimilarityToOrigin(const Mat& particleRoi)
{
	Mat hist = generateHist(particleRoi);
	Mat distResult = originHist.mul(hist);
	sqrt(distResult, distResult);
	double distanceSum = sum(distResult).val[0];
	double pdf = 1/(sqrt(2*Utility::PI)*noiseWeight)*exp(-(1-distanceSum)/2/(noiseWeight*noiseWeight));
	return pdf;
}

int RGBHistFeature::GenerateOriginFeature(vector<Mat>& roi)
{
	vector<Mat>::iterator roiIter;
	Mat hist;
	for(roiIter=roi.begin(); roiIter!=roi.end(); roiIter++)
	{
		if(roiIter == roi.begin())
		{
			hist = generateHist(*roiIter);
		}
		else
		{
			hist += generateHist(*roiIter);
		}
	}
	originHist = hist/roi.size();
	
	return 0;
}

Mat RGBHistFeature::generateHist(const Mat& roi)
{
	float hranges[] = {0, 256};
	float sranges[] = {0, 256};
	float vranges[] = {0, 256};
	const float* phranges[] = {hranges, sranges, vranges};
	int channels[3] = {0, 1, 2};
	Mat hist;
	calcHist(&roi, 1, channels, Mat(), hist, 3, histSize, phranges);
	Scalar histSum = sum(hist);
	hist = hist / histSum.val[0];

	return hist;
}