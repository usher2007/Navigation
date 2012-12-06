#pragma  once
#include "Particle.h"
#include "ParticleFeature.h"

using namespace cv;
class TrackedPerson
{
public:
	TrackedPerson(const Mat& image, const Rect& location, const int ParticleNum, const int Id);
	int AddParticleFeature(ParticleFeature* feature);
	int AddInitialRoi(const Mat& image, const Rect& location);
	Rect GetCurrentLocation() const;
	int GetId();
	int ResampleParticleList();
	int CalcParticleWeight(const Mat& image);
	int UpdateCurrentLocation();
private:
	int generateParticleList();
private:
	std::vector<ParticleFeature *> features;
	std::vector<Particle> particleList;
	Mat initialImage;
	vector<Mat> rois;
	Rect initialLocation;
	Rect currentLocation;
	int particleNum;
	int id;
};