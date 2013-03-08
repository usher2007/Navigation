#include "stdafx.h"
#include "Person.h"
#include "Utility.h"
//Utility Init
std::string TrackingConfig::VIDEO_FILE_NAME = "H:\\GitHubCode\\Navigation\\Research\\TrackingTeacher\\Data\\T_WALK4.mp4";
std::string TrackingConfig::RESULT_FILE_NAME = "H:\\GitHubCode\\Navigation\\Research\\TrackingByGBM\\Data\\12.12\\T_WALK4.avi";
cv::Rect TrackingConfig::BEGIN_TRACKING_AREA(120, 115, 480, 346);
cv::Rect TrackingConfig::STOP_TRACKING_AREA(90, 72, 540, 432);
int TrackingConfig::DISAPPEAR_FRAME_THRESH = 10;
int TrackingConfig::LEAST_HUMAN_GAP = 50;
int TrackingConfig::HUMAN_WIDTH = 30;
int TrackingConfig::CENTER_WEIGHT_THRESH = 25;
int TrackingConfig::TRACK_INTERVAL = 2;
int TrackingConfig::FG_LOW_THRESH = 128;
int TrackingConfig::FG_UP_THRESH = 255;
double TrackingConfig::GBM_LEARNING_RATE = 0.01;
double TrackingConfig::FG_HIST_THRESH = 2;
double TrackingConfig::PI = 3.1415926;
//For Debug
cv::Rect TrackingConfig::PAD_AREA_1(190, 120, 20, 300);
cv::Rect TrackingConfig::PAD_AREA_2(232, 120, 20, 300);
cv::Rect TrackingConfig::PAD_AREA_3(365, 120, 20, 300);
cv::Rect TrackingConfig::PAD_AREA_4(408, 120, 20, 300);
// Person
Person::Person(const cv::Point2f& initialCenter) : baryCenter(initialCenter), detectedTimes(0), disappearTimes(0), detectedOrTracked(true)
{
}

cv::Point2f Person::GetBaryCenter()
{
	return baryCenter;
}

int Person::GetCenterDistance(const cv::Point2f& newCenter)
{
	cv::Point2f dist = newCenter - baryCenter;
	int result = dist.x*dist.x + dist.y*dist.y;
	if(result < 2500)
	{
		return result;
	}
	return -1;
}

int Person::UpdateBaryCenter(const cv::Point2f& newCenter)
{
	baryCenter = newCenter;
	return 0;
}

bool Person::Disappeared()
{
	return disappearTimes > TrackingConfig::DISAPPEAR_FRAME_THRESH;
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
	std::vector<Person>::iterator personIter;
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

int PersonManager::ProcessBaryCenter(const cv::Point2f& baryCenter)
{
	std::vector<Person>::iterator personIter, specialTrackIter, specialDetectIter;
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

int PersonManager::DrawPersons(cv::Mat& image)
{
	std::vector<Person>::iterator personIter;
	/*for(personIter=detectedPersons.begin(); personIter!=detectedPersons.end(); ++personIter)
	{
		circle(image, personIter->GetBaryCenter(), 4, Scalar(0, 255, 0), 2);
	}*/
	for(personIter=trackedPersons.begin(); personIter!=trackedPersons.end(); ++personIter)
	{
		cv::circle(image, personIter->GetBaryCenter(), 4, cv::Scalar(0,255, 255), 2);
	}
	return 0;
}

int PersonManager::updateDetectedPersons()
{
	if(detectedPersons.empty())
	{
		return -1;
	}
	std::vector<Person>::iterator personIter = detectedPersons.begin();
	while(personIter!=detectedPersons.end())
	{
		personIter->UpdateDetectAndDisappearTimes();
		if(personIter->GetBaryCenter().inside(TrackingConfig::BEGIN_TRACKING_AREA))
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
	std::vector<Person>::iterator personIter = trackedPersons.begin();
	while(personIter!=trackedPersons.end())
	{
		personIter->UpdateDetectAndDisappearTimes();
		if(personIter->IsNoise() || (personIter->Disappeared() && !(personIter->GetBaryCenter().inside(TrackingConfig::STOP_TRACKING_AREA))))
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

std::vector<cv::Point2f> PersonManager::GetTrackedPersons()
{
	std::vector<cv::Point2f> trackedPersonCenters;
	std::vector<Person>::iterator personIter;
	for(personIter=trackedPersons.begin(); personIter!=trackedPersons.end(); ++personIter)
	{
		trackedPersonCenters.push_back(personIter->GetBaryCenter());
	}
	
	return trackedPersonCenters;
}

