#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ParticleTrackingAlg.h"
#include "RGBHistFeature.h"
#include "Particle.h"
#include "ParticleFeature.h"
#include "Utility.h"
#include <time.h>
using namespace cv;

const string videoFileName = "H:\\GitHubCode\\Navigation\\Research\\ParticleFiltering\\FromShunli\\Video\\MVI_2132.avi";
bool selectObject = false;
Rect selection;
Point origin;
Mat image;
int trackObject = 0;

void onMouse( int event, int x, int y, int, void* )
{
	if( selectObject )
	{
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = std::abs(x - origin.x);
		selection.height = std::abs(y - origin.y);

		selection &= Rect(0, 0, image.cols, image.rows);
	}

	switch( event )
	{
	case CV_EVENT_LBUTTONDOWN:
		origin = Point(x,y);
		selection = Rect(x,y,0,0);
		selectObject = true;
		break;
	case CV_EVENT_LBUTTONUP:
		selectObject = false;
		if( selection.width > 0 && selection.height > 0 )
			trackObject = -1;
		break;
	}
}
int main(int argc, char **argv)
{
	VideoCapture cap;
	Mat frame;
	
	cap.open(videoFileName);
	cap >> frame;
	frame.copyTo(image);
	namedWindow("Select Object", CV_WINDOW_AUTOSIZE);
	setMouseCallback("Select Object", onMouse, 0);
	imshow("Select Object", image);

	for(;;)
	{
		Mat tmpImage;
		if(trackObject < 0)
		{
			break;
		}
		if( selectObject && selection.width > 0 && selection.height > 0 )
		{
			image.copyTo(tmpImage);
			rectangle(tmpImage,selection, Scalar(0,0,0));
			imshow("Select Object", tmpImage);
		}
		else
		{
			imshow("Select Object", image);
		}
		waitKey(10);
	}

	ParticleTrackingAlg particleTrackingAlg(Utility::particleNum);
	RGBHistFeature *pRGBHistFeature = new RGBHistFeature();
	particleTrackingAlg.AddParticleFeature(pRGBHistFeature);
	particleTrackingAlg.SetInitialObject(selection, image);
	long startTime = 0, endTime = 0;
	startTime = clock();
	particleTrackingAlg.Tracking(cap);
	endTime = clock();
	double totalTime = (endTime - startTime)/CLOCKS_PER_SEC;
	return 0;
}