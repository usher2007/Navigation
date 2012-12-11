#include "Person.h"
#include "Utility.h"
// Person
Person::Person(const Point2f& initialCenter) : baryCenter(initialCenter), detectedTimes(0), disappearTimes(0), detectedOrTracked(true)
{
}

Point2f Person::GetBaryCenter()
{
	return baryCenter;
}

int Person::GetCenterDistance(const Point2f& newCenter)
{
	Point2f dist = newCenter - baryCenter;
	int result = dist.x*dist.x + dist.y*dist.y;
	if(result < 2500)
	{
		return result;
	}
	return -1;
}

int Person::UpdateBaryCenter(const Point2f& newCenter)
{
	baryCenter = newCenter;
	return 0;
}

bool Person::Disappeared()
{
	return disappearTimes > Utility::DisapperFrameThresh;
}

bool Person::IsNoise()
{
	if(disappearTimes == 0)
	{
		return false;
	}
	else
	{
		return detectedTimes*1.0/disappearTimes < 5;
	}
}

int Person::SetCurrentStatus(bool detectedOrTracked)
{
	this->detectedOrTracked = detectedOrTracked;
	return 0;
}

bool Person::IsDetectedOrTracked()
{
	return detectedOrTracked;
}

int Person::UpdateDetectAndDisappearTimes()
{
	if(detectedOrTracked)
	{
		disappearTimes = 0;
		detectedTimes++;
	}
	else
	{
		disappearTimes++;
	}
	return 0;
}

// Person Manager
PersonManager::PersonManager()
{

}

int PersonManager::ResetAllPersonStatus()
{
	vector<Person>::iterator personIter;
	for(personIter=detectedPersons.begin(); personIter!=detectedPersons.end(); ++personIter)
	{
		personIter->SetCurrentStatus(false);
	}
	for(personIter=trackedPersons.begin(); personIter!=trackedPersons.end(); ++personIter)
	{
		personIter->SetCurrentStatus(false);
	}
	return 0;
}

int PersonManager::ProcessBaryCenter(const Point2f& baryCenter)
{
	vector<Person>::iterator personIter, specialTrackIter, specialDetectIter;
	int dist = 0;
	int smallestTrackDist = INT_MAX;
	// First update the tracked person.
	for(personIter=trackedPersons.begin(); personIter!=trackedPersons.end(); ++personIter)
	{
		dist = personIter->GetCenterDistance(baryCenter);
		if(dist>=0 && dist<smallestTrackDist)
		{
			smallestTrackDist=dist;
			specialTrackIter = personIter;
		}
	}
	// Then update the detected person.
	dist = 0;
	int smallestDetectDist = INT_MAX;
	for(personIter=detectedPersons.begin(); personIter!=detectedPersons.end(); ++personIter)
	{
		dist = personIter->GetCenterDistance(baryCenter);
		if(dist>=0 && dist<smallestDetectDist)
		{
			smallestDetectDist=dist;
			specialDetectIter=personIter;
		}
	}
	//Update Barycenter
	if(smallestTrackDist < INT_MAX || smallestDetectDist < INT_MAX)
	{
		if(smallestTrackDist < INT_MAX && smallestDetectDist < INT_MAX)
		{
			if(smallestTrackDist > 2*smallestDetectDist)
			{
				specialDetectIter->UpdateBaryCenter(baryCenter);
				specialDetectIter->SetCurrentStatus(true);
			}
			else
			{
				specialTrackIter->UpdateBaryCenter(baryCenter);
				specialTrackIter->SetCurrentStatus(true);
			}
		}
		else if(smallestDetectDist < INT_MAX)
		{
			specialDetectIter->UpdateBaryCenter(baryCenter);
			specialDetectIter->SetCurrentStatus(true);
		}
		else if(smallestTrackDist < INT_MAX)
		{
			specialTrackIter->UpdateBaryCenter(baryCenter);
			specialTrackIter->SetCurrentStatus(true);
		}
		return 0;
	}
	else
	{
		Person p(baryCenter);
		detectedPersons.push_back(p);
		return 1;
	}
}

int PersonManager::Update()
{
	updateDetectedPersons();
	updateTrackedPersons();
	return 0;
}

int PersonManager::DrawPersons(Mat& image)
{
	vector<Person>::iterator personIter;
	for(personIter=detectedPersons.begin(); personIter!=detectedPersons.end(); ++personIter)
	{
		circle(image, personIter->GetBaryCenter(), 4, Scalar(0, 255, 0), 2);
	}
	for(personIter=trackedPersons.begin(); personIter!=trackedPersons.end(); ++personIter)
	{
		circle(image, personIter->GetBaryCenter(), 4, Scalar(100,100, 0), 2);
	}
	return 0;
}

int PersonManager::updateDetectedPersons()
{
	if(detectedPersons.empty())
	{
		return -1;
	}
	vector<Person>::iterator personIter = detectedPersons.begin();
	while(personIter!=detectedPersons.end())
	{
		personIter->UpdateDetectAndDisappearTimes();
		if(personIter->GetBaryCenter().inside(Utility::BeginTrackingArea))
		{
			Person p(*personIter);
			trackedPersons.push_back(p);
			personIter = detectedPersons.erase(personIter);
		}
		else
		{
			++personIter;
		}
	}
	return 0;
}

int PersonManager::updateTrackedPersons()
{
	if(trackedPersons.empty())
	{
		return -1;
	}
	vector<Person>::iterator personIter = trackedPersons.begin();
	while(personIter!=trackedPersons.end())
	{
		personIter->UpdateDetectAndDisappearTimes();
		if(personIter->IsNoise() || (personIter->Disappeared() && !(personIter->GetBaryCenter().inside(Utility::StopTrackingArea))))
		{
			personIter = trackedPersons.erase(personIter);
		}
		else
		{
			++personIter;
		}
	}
	return 0;
}

