#include "ParticleTrackingAlg.h"

ParticleTrackingAlg::ParticleTrackingAlg()
{
	
}

int ParticleTrackingAlg::AddTrackedPerson(TrackedPerson person)
{
	personList.push_back(person);
	return 0;
}

bool ParticleTrackingAlg::PersonAlreadyTracked(const Point2f& center)
{
	vector<TrackedPerson>::iterator personIter;
	for(personIter=personList.begin(); personIter!=personList.end(); personIter++)
	{
		Point2f personCenter;
		Rect personLoc = personIter->GetCurrentLocation();
		personCenter.x = personLoc.x + personLoc.width/2;
		personCenter.y = personLoc.y + personLoc.height/2;
		if((center.x-personCenter.x)*(center.x-personCenter.x)+(center.y-personCenter.y)*(center.y-personCenter.y) < 2500)
		{
			return true;
		}
	}
	return false;
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




