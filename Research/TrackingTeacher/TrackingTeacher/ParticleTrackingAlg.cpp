#include "ParticleTrackingAlg.h"
#include "Utility.h"

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
	vector<TrackedPerson>::iterator personIter = personList.begin();
	while(personIter!=personList.end())
	{
		personIter->CalcParticleWeight(image);
		personIter->UpdateCurrentLocation();
		personIter->ResampleParticleList();
		Rect currentLoc = personIter->GetCurrentLocation();
		Point2f currentCenter(currentLoc.x+currentLoc.width/2, currentLoc.y+currentLoc.height/2);
		if(!currentCenter.inside(Utility::trackingArea))
		{
			personIter = personList.erase(personIter);
		}
		else
		{
			personIter++;
		}
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

int ParticleTrackingAlg::DrawPersons(Mat& frame)
{
	vector<TrackedPerson>::iterator personIter;
	for(personIter=personList.begin(); personIter!=personList.end(); personIter++)
	{
		Rect personLoc = personIter->GetCurrentLocation();
		rectangle(frame, personLoc,Scalar(100,100,0), 2);
	}
	return 0;
}




