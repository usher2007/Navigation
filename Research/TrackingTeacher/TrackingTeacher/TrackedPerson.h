#pragma  once
#include "Particle.h"
#include "ParticleFeature.h"

using namespace cv;
class TrackedPerson
{
public:
	TrackedPerson(const Mat& image, const Rect& location, const int ParticleNum, const int Id);
	int AddParticleFeature(ParticleFeature* feature);
	Rect GetCurrentLocation();
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
	Rect initialLocation;
	Rect currentLocation;
	int particleNum;
	int id;
};