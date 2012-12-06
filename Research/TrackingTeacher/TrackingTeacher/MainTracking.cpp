#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "ParticleTrackingAlg.h"
#include "RGBHistFeature.h"
#include "Particle.h"
#include "ParticleFeature.h"
#include "TrackedPerson.h"
#include "Utility.h"
#include <time.h>
#include <iostream>
using namespace cv;

const string videoFileName = "H:\\GitHubCode\\Navigation\\Research\\TrackingTeacher\\Data\\T_WALK1.mp4";
int personId = 0;
int AddBaryCenterToPerson(vector<TrackedPerson>& detectedPersons, const Point2f& center, const Mat& image);
bool CenterInPerson(const Point2f& center, const TrackedPerson& detectedPerson);
int main(int argc, char **argv)
{
	VideoCapture cap;
	Mat frame, foreground, tmpForeground;
	BackgroundSubtractorMOG2 mog;
	cap.open(videoFileName);
	cap >> frame;
	VideoWriter videoWriter("H:\\GitHubCode\\Navigation\\Research\\TrackingTeacher\\Data\\BaryCenter\\BaryCenter.avi", CV_FOURCC('X','V','I','D'),25,frame.size());
	int cols = frame.cols;
	int rows = frame.rows;
	Rect trackingArea(rows/7, cols/9, rows - rows*2/7, cols - cols*2/9);
	Rect notDetectArea(rows/6, cols/8, rows - rows/3, cols - cols/4);
	//namedWindow("Gaussian Foreground");
	//namedWindow("Bary Centre");
	namedWindow("Result");
	Utility::GenerateAuxMatForBaryCentre(rows, cols);
	vector<TrackedPerson> detectedPersons;
	ParticleTrackingAlg particleTrackingAlg;
	for(int index=0; ;index++)
	{
		if(frame.empty())
		{
			break;
		}
		mog(frame, foreground, 0.01);
		threshold(foreground, foreground, 128, 255, THRESH_BINARY);
		if(index == 0)
		{
			cap>>frame;
			continue;
		}
		foreground.copyTo(tmpForeground);
		tmpForeground = tmpForeground/255;

		vector<Point2f> centres;
		Utility::CalcImageBaryCentre(tmpForeground, centres);
		for(vector<Point2f>::iterator itCenter = centres.begin(); itCenter!= centres.end(); itCenter++)
		{
			if(itCenter->inside(notDetectArea) || particleTrackingAlg.PersonAlreadyTracked(*itCenter))
			{
				continue;
			}
			int curPersonId = AddBaryCenterToPerson(detectedPersons, *itCenter, frame);
			if(itCenter->inside(trackingArea))
			{
				for(vector<TrackedPerson>::iterator personIter=detectedPersons.begin(); personIter!=detectedPersons.end(); personIter++)
				{
					if(personIter->GetId() == curPersonId)
					{
						particleTrackingAlg.AddTrackedPerson(*personIter);
						detectedPersons.erase(personIter);
					}
				}
			}
		}
		particleTrackingAlg.Tracking(frame);
		imshow("Guassian Foreground", foreground);
		//imshow("Bary Centre", tmpForeground);
		rectangle(frame, trackingArea, Scalar(0,255,0),2);
		rectangle(frame, notDetectArea, Scalar(0,0,255), 2);
		imshow("Result", frame);
		waitKey(10);

		/*char *fileName = new char[1024];
		sprintf(fileName, "H:\\GitHubCode\\Navigation\\Research\\TrackingTeacher\\Data\\BaryCenter\\%d.bmp", index);
		imwrite(fileName, frame);*/
		videoWriter << frame;

		cap >> frame;
	}
}

int AddBaryCenterToPerson(vector<TrackedPerson>& detectedPersons, const Point2f& center, const Mat& image)
{
	vector<TrackedPerson>::iterator personIter;
	int left = (center.x-50 > 0) ? (center.x-50) : 0;
	int right = (center.x+50 <image.cols) ? (center.x+50) : image.cols-1;
	int top = (center.y-80 > 0) ? (center.y-80) : 0;
	int bottom = (center.y+80 < image.rows) ? (center.y+80) : image.rows-1;
	Rect currentPerson(left, top, right-left, bottom-top);
	for(personIter=detectedPersons.begin(); personIter!=detectedPersons.end(); personIter++)
	{
		if(CenterInPerson(center, *personIter))
		{
			personIter->AddInitialRoi(image, currentPerson);
			return personIter->GetId();
		}
	}
	TrackedPerson detectedPerson(image, currentPerson, 50, personId++);
	detectedPersons.push_back(detectedPerson);
	return personId-1;
}

bool CenterInPerson(const Point2f& center, const TrackedPerson& detectedPerson)
{
	Rect detectedPersonLoc = detectedPerson.GetCurrentLocation();
	Point2f detectedPersonCenter(detectedPersonLoc.x + detectedPersonLoc.width/2, detectedPersonLoc.y + detectedPersonLoc.height/2);
	if(((detectedPersonCenter.x-center.x)*(detectedPersonCenter.x-center.x) + (detectedPersonCenter.y-center.y)*(detectedPersonCenter.y-center.y))<2500)
	{
		return true;
	}
	return false;
}