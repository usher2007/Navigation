#include "ParticleTracking.h"
#include <time.h>
using namespace cv;

Mat image;
int trackObject = 0;
bool selectObject = false;
Rect selection;
Point origin;
int histSize = 16;
int CalcHsvHist(Mat &hist, Mat image, Rect &selectRoi, const float *phranges);
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
	Mat frame, hsv, hue, mask;
	float hranges[] = {0,180};
	const float* phranges = hranges;
	cap >> frame;
	frame.copyTo(image);
	namedWindow( "Particle Demo", 0 );
	setMouseCallback( "Particle Demo", onMouse, 0 );
	imshow("Particle Demo", image);

	for(;;)
	{
		if(trackObject < 0)
		{
			trackingAlg.originParticle.xPos = selection.x;
			trackingAlg.originParticle.yPos = selection.y;
			trackingAlg.originParticle.width = selection.width;
			trackingAlg.originParticle.height = selection.height;
			trackingAlg.originParticle.weight = 0.0;
			break;
		}
		if( selectObject && selection.width > 0 && selection.height > 0 )
		{
			Mat roi(image, selection); 
			bitwise_not(roi, roi);
		}
		imshow("Particle Demo", image);
		waitKey(10);
	}

	trackingAlg.particleGroup.generateParticleList(trackingAlg.originParticle, 20);

	Rect lastObjectRegion;
	frame.copyTo(image);
	lastObjectRegion.x = trackingAlg.originParticle.xPos;
	lastObjectRegion.y = trackingAlg.originParticle.yPos;
	lastObjectRegion.width = trackingAlg.originParticle.width;
	lastObjectRegion.height = trackingAlg.originParticle.height;
	CalcHsvHist(trackingAlg.originHist, image, lastObjectRegion, phranges);
	for(;;)
	{
		cap >> frame;
		if(frame.empty())
		{
			break;
		}
		frame.copyTo(image);
		lastObjectRegion.x = trackingAlg.originParticle.xPos;
		lastObjectRegion.y = trackingAlg.originParticle.yPos;
		lastObjectRegion.width = trackingAlg.originParticle.width;
		lastObjectRegion.height = trackingAlg.originParticle.height;
		CalcHsvHist(trackingAlg.originHist, image, lastObjectRegion, phranges);
	}
}

int CalcHsvHist(Mat &hist, Mat image, Rect &selectRoi, const float *phranges)
{
	Mat hsv,  hue, mask;
	cvtColor(image, hsv, CV_BGR2HSV);
	inRange(hsv, Scalar(0, 0, 0),
		Scalar(180, 256, 256), mask);
	hue.create(hsv.size(), hsv.depth());
	int ch[] = {0, 0};
	mixChannels(&hsv, 1, &hue, 1, ch, 1);

	Mat roi(hue, selectRoi), maskroi(mask, selectRoi);
	calcHist(&roi, 1, 0, maskroi,hist, 1, &histSize, &phranges);
	normalize(hist, hist, 0, 255, CV_MINMAX);
	return 0;
}

double ParticleTrackingAlg::calcDistance(const Mat &histOrig, const Mat &histTmp)
{
	double distanceSum = 0.0, firstLen = 0.0, secondLen = 0.0;
	for(int i=0; i<histSize; i++)
	{
		firstLen += (histOrig.data[i] * histOrig.data[i]);
		secondLen += (histTmp.data[i] * histTmp.data[i]);
		double dist = histOrig.data[i] - histTmp.data[i];
		distanceSum += (dist * dist);
	}
	return sqrt(distanceSum)/(sqrt(firstLen * secondLen));
}

int ParticleGroup::normalize()
{
	std::vector<Particle>::iterator particleIter; 
	double weightSum = 0.0;
	for(particleIter = ParticleList.begin(); particleIter != ParticleList.end(); ++particleIter)
	{
		weightSum += particleIter->weight;
	}

	for(particleIter = ParticleList.begin(); particleIter != ParticleList.end(); ++particleIter)
	{
		particleIter->weight /= weightSum;
	}
	return 0;
}

double getRand(double min, double max)
{
	int randRaw = rand()%100 + 1;
	double result = (randRaw - 1)*1.0/(100 - 1)*(max - min) + min;
	return result;
}

int ParticleGroup::resampleParticle()
{
	int particleNum = ParticleList.size();
	double *cumPdf = new double[particleNum];
	int *newParticleIndex = new int [particleNum];
	std::vector<Particle>::iterator particleIter;
	std::vector<Particle> newParticleList;
	double sum = 0.0;
	int index = 0;
	for(particleIter = ParticleList.begin(); particleIter != ParticleList.end(); ++particleIter)
	{
		sum += particleIter->weight;
		cumPdf[index] = sum;
		index++;
	}

	double randNum = 0.0;
	srand(time(0));
	particleIter = ParticleList.begin();
	RNG rng;
	for(int i=0; i<particleNum; i++)
	{
		randNum = getRand(0.0, sum);
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
		newParticleIndex[i] = j;
		Particle p((particleIter+j)->xPos, (particleIter+j)->yPos, (particleIter+j)->width, (particleIter+j)->height);
		p.weight = (particleIter+j)->weight;
		p.xPos = p.xPos + rng.gaussian(xNoise);
		p.yPos = p.yPos + rng.gaussian(yNoise);
		newParticleList.push_back(p);
	}

	ParticleList = newParticleList;
	return 0;
}

int ParticleGroup::generateParticleList(const Particle &originParticle, int num)
{
	RNG rng;
	for(int i=0; i<num; i++)
	{
		Particle p(originParticle.xPos, originParticle.yPos, originParticle.width, originParticle.height);
		p.weight = originParticle.weight;
		p.xPos = p.xPos + rng.gaussian(xNoise);
		p.yPos = p.yPos + rng.gaussian(yNoise);
		ParticleList.push_back(p);
	}

	return 0;
}