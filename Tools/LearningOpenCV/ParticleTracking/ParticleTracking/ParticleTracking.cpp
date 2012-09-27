#include "ParticleTracking.h"
using namespace cv;

Mat image;
int trackObject = 0;
bool selectObject = false;
Rect selection;
Point origin;

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
	cap.open(videoFileName);

	ParticleTrackingAlg trackingAlg;
	Mat frame;
	cap >> frame;
	frame.copyTo(image);
	namedWindow( "Particle Demo", 0 );
	imshow("Particle Demo", frame);
	setMouseCallback( "Particle Demo", onMouse, 0 );
	for(;;)
	{
		if(trackObject < 0)
		{
			
			break;
		}
	}
	for(;;)
	{
		cap >> frame;
		if(frame.empty())
		{
			break;
		}

	}
}