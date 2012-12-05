#include "ParticleTrackingAlg.h"

ParticleTrackingAlg::ParticleTrackingAlg()
{
	
}

int ParticleTrackingAlg::AddTrackedPerson(TrackedPerson person)
{
	personList.push_back(person);
	return 0;
}

int ParticleTrackingAlg::Tracking(const Mat& image)
{
	vector<TrackedPerson>::iterator personIter;
	for(personIter=personList.begin(); personIter!=personList.end(); personIter++)
	{
		personIter->CalcParticleWeight(image);
		personIter->UpdateCurrentLocation();
		personIter->ResampleParticleList();
	}
	return 0;
}

Rect ParticleTrackingAlg::GetPersonLocationById(const int id)
{
	vector<TrackedPerson>::iterator personIter;
	for(personIter=personList.begin(); personIter!=personList.end(); personIter++)
	{
		if(personIter->GetId() == id)
		{
			return personIter->GetCurrentLocation();
		}
	}
	return Rect(0,0,0,0);
}




