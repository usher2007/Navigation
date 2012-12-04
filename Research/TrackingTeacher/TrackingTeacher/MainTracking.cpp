#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "ParticleTrackingAlg.h"
#include "RGBHistFeature.h"
#include "Particle.h"
#include "ParticleFeature.h"
#include "Utility.h"
#include <time.h>
#include <iostream>
using namespace cv;

const string videoFileName = "H:\\GitHubCode\\Navigation\\Research\\TrackingTeacher\\Data\\T_WALK1.mp4";

int main(int argc, char **argv)
{
	VideoCapture cap;
	Mat frame, foreground, tmpForeground;
	BackgroundSubtractorMOG2 mog;
	cap.open(videoFileName);
	cap >> frame;
	int cols = frame.cols;
	int rows = frame.rows;
	namedWindow("Gaussian Foreground");
	namedWindow("Bary Centre");
	Utility::GenerateAuxMatForBaryCentre(rows, cols);
	
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

		int sum = Utility::GetNoneZeroPointsNum(tmpForeground);
		vector<Point2f> centres;
		Utility::CalcImageBaryCentre(tmpForeground, centres);
		std::cout<<"None Zero Points: "<<sum<<std::endl;
		//std::cout<<"Bary Centre: "<<centres<<std::endl;
		for(vector<Point2f>::iterator itCenter = centres.begin(); itCenter!= centres.end(); itCenter++)
		{
			circle(tmpForeground, *itCenter, 5, Scalar(255,0,0), 2);
		}
		imshow("Guassian Foreground", foreground);
		//char *fileName = new char[1024];
		//sprintf(fileName, "H:\\GitHubCode\\Navigation\\Research\\TrackingTeacher\\Data\\GassianFore\\%d.bmp", index);
		//imwrite(fileName, foreground);
		imshow("Bary Centre", tmpForeground);
		waitKey(10);
		cap >> frame;
	}
}