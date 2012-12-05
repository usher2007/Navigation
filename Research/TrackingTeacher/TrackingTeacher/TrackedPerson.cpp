#include "TrackedPerson.h"
#include "Utility.h"

TrackedPerson::TrackedPerson(const Mat& image, const Rect& location, const int ParticleNum, const int Id)
{
	image.copyTo(initialImage);
	initialLocation = location;
	currentLocation = location;
	particleNum = ParticleNum;
	id = Id;
	AddInitialRoi(initialImage, initialLocation);
	generateParticleList();
}

int TrackedPerson::AddInitialRoi(const Mat& image, const Rect& location)
{
	Mat roi(image, location);
	rois.push_back(roi);
	return 0;
}

Rect TrackedPerson::GetCurrentLocation()
{
	return currentLocation;
}

int TrackedPerson::AddParticleFeature(ParticleFeature* feature)
{
	Mat roi(initialImage, initialLocation);
	(*feature).GenerateOriginFeature(rois);
	features.push_back(feature);
	return 0;
}

int TrackedPerson::GetId()
{
	return id;
}

int TrackedPerson::CalcParticleWeight(const Mat& image)
{
	vector<Particle>::iterator particleIter;
	vector<ParticleFeature *>::iterator featureIter;
	double sumPdf = 0;
	for(particleIter=particleList.begin(); particleIter!=particleList.end(); particleIter++)
	{
		Mat particleRoi = particleIter->GetParticleRoi(image);
		double particleWeight = 0;
		for(featureIter=features.begin(); featureIter!=features.end(); featureIter++)
		{
			double pdf = ((*featureIter)->CalcSimilarityToOrigin(particleRoi)) * ((*featureIter)->GetFeatureWeight());
			//For multi-feature, the sumPdf should be specified to one feature.
			sumPdf += pdf;
			particleWeight += pdf;
		}
		particleIter->SetParticleWeight(particleWeight);
	}
	for(particleIter=particleList.begin(); particleIter!=particleList.end(); particleIter++)
	{
		double weight = particleIter->GetParticleWeight() / sumPdf;
		particleIter->SetParticleWeight(weight);
	}

	return 0;
}

int TrackedPerson::UpdateCurrentLocation()
{
	double newX = 0.0, newY = 0.0;
	vector<Particle>::iterator particleIter;
	for(particleIter=particleList.begin(); particleIter!=particleList.end(); particleIter++)
	{
		newX += particleIter->GetParticleWeight() * particleIter->GetParticleRegion().x;
		newY += particleIter->GetParticleWeight() * particleIter->GetParticleRegion().y;
	}
	currentLocation.x = newX;
	currentLocation.y = newY;
	return 0;
}

int TrackedPerson::ResampleParticleList()
{
	double *cumPdf = new double[particleNum];
	std::vector<Particle> newParticleList;

	std::vector<Particle>::iterator particleIter;
	double sumWeight = 0.0;
	int index = 0;
	for(particleIter=particleList.begin(); particleIter!=particleList.end(); ++particleIter)
	{
		sumWeight += particleIter->GetParticleWeight();
		cumPdf[index] = sumWeight;
		index++;
	}

	double randNum = 0.0;
	particleIter = particleList.begin();
	RNG rng;
	for(int i=0; i<particleNum; i++)
	{
		randNum = rng.uniform(0.0, sumWeight);
		int j;
		for(j=0; j<particleNum; j++)
		{
			if(cumPdf[j] < randNum)
			{
				continue;
			}
			else
			{
				break;
			}
		}
		Particle p((particleIter+j)->GetParticleRegion(), rng.gaussian(Utility::xNoise), rng.gaussian(Utility::yNoise));
		p.SetParticleWeight((particleIter+j)->GetParticleWeight());
		newParticleList.push_back(p);
	}
	particleList = newParticleList;
	return 0;
}

int TrackedPerson::generateParticleList()
{
	RNG rng;
	for(int i=0; i<particleNum; i++)
	{
		Particle p(initialLocation.x + rng.gaussian(Utility::xNoise), initialLocation.y + rng.gaussian(Utility::yNoise), 
			initialLocation.width, initialLocation.height);
		particleList.push_back(p);
	}
	return 0;
}

