#include "ParticleTracking.h"
#include <time.h>
using namespace cv;

int CalcHsvHist(Mat &hist, Mat image, Rect &selectRoi);
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
	cap >> frame;
	frame.copyTo(image);
	namedWindow( "Particle Demo", CV_WINDOW_AUTOSIZE );
	namedWindow( "Debug", CV_WINDOW_AUTOSIZE);
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

	trackingAlg.particleGroup.generateParticleList(trackingAlg.originParticle, particleNum);
	double particleWeight[particleNum];

	Rect lastObjectRegion;
	cap >> frame;
	frame.copyTo(image);
	lastObjectRegion.x = trackingAlg.originParticle.xPos;
	lastObjectRegion.y = trackingAlg.originParticle.yPos;
	lastObjectRegion.width = trackingAlg.originParticle.width;
	lastObjectRegion.height = trackingAlg.originParticle.height;
	lastObjectRegion &= Rect(0, 0, image.cols, image.rows);
	CalcHsvHist(trackingAlg.originHist, image, lastObjectRegion);
	int frameCount = 0;
	double dist = 0.0;
	double pdf = 0.0;
	for(;;)
	{
		cap >> frame;
		if(frame.empty())
		{
			break;
		}
		frame.copyTo(image);
		vector<Particle>::iterator particleIter;
		double sumpdf = 0.0;
		int index = 0;
		for(particleIter = trackingAlg.particleGroup.ParticleList.begin();particleIter != trackingAlg.particleGroup.ParticleList.end(); particleIter++)
		{
			Rect particleRegion;
			particleRegion.x = particleIter->xPos;
			particleRegion.y = particleIter->yPos;
			particleRegion.width = particleIter->width;
			particleRegion.height = particleIter->height;
			particleRegion &= Rect(0, 0, image.cols, image.rows);
			CalcHsvHist(trackingAlg.hist, image, particleRegion);
			dist = trackingAlg.calcDistance(trackingAlg.originHist, trackingAlg.hist);
			pdf = 1/(sqrt(2*PI)*noiseWeight)*exp(-(1-dist)/2/(noiseWeight*noiseWeight));
			sumpdf += pdf;
			particleWeight[index] = pdf;
			index++;
		}

		for(int i=0; i<particleNum; i++)
		{
			particleWeight[i] /= sumpdf;
			trackingAlg.particleGroup.ParticleList[i].weight = particleWeight[i];
		}
		index = 0;
		double newX = 0.0;
		double newY =0.0;
		for(particleIter = trackingAlg.particleGroup.ParticleList.begin();particleIter != trackingAlg.particleGroup.ParticleList.end(); particleIter++)
		{
			newX += particleWeight[index] * particleIter->xPos;
			newY += particleWeight[index] * particleIter->yPos;
			index++;
		}
		trackingAlg.particleGroup.resampleParticle();

		trackingAlg.originParticle.xPos = newX;
		trackingAlg.originParticle.yPos = newY;
		lastObjectRegion.x = trackingAlg.originParticle.xPos;
		lastObjectRegion.y = trackingAlg.originParticle.yPos;
		//std::cout<<"Frame Count: "<<frameCount<<"\n"<<"X:"<<newX<<" Y:"<<newY<<std::endl;
		frameCount ++;
		Point pt1, pt2;
		pt1.x = newX;
		pt1.y = newY;
		pt2.x = newX + trackingAlg.originParticle.width;
		pt2.y = newY + trackingAlg.originParticle.height;
		rectangle(image, pt1, pt2, Scalar(0,0,0));
		Mat hsv, hue;
		cvtColor(image, hsv, CV_BGR2HSV);
		inRange(hsv, Scalar(0, 30, 10),
			Scalar(180, 256, 256), mask);
		hue.create(hsv.size(), hsv.depth());
		int ch[] = {0, 0};
		mixChannels(&hsv, 1, &hue, 1, ch, 1);
		imshow("Particle Demo", image);
		//imshow("Debug", hue);
		waitKey(2);
	}
}

int CalcHsvHist(Mat &hist, Mat image, Rect &selectRoi)
{
	Mat hsv,  hue, mask;
	cvtColor(image, hsv, CV_BGR2HSV);
	inRange(hsv, Scalar(0, 30, 10),
		Scalar(256, 256, 256), mask);
	hue.create(hsv.size(), hsv.depth());
	int ch[] = {0, 0};
	mixChannels(&hsv, 1, &hue, 1, ch, 1);
	//Mat roi(hue, selectRoi), maskroi(mask, selectRoi);
	Mat roi(hsv, selectRoi), maskroi(mask, selectRoi);
	int channels[3] = {0, 1, 2};
	calcHist(&roi, 1, channels, Mat(), hist, 3, histSize, phranges);
	Scalar histSum = sum(hist);

	hist = hist / histSum.val[0];
	return 0;
}

double ParticleTrackingAlg::calcDistance(const Mat &histOrig, const Mat &histTmp)
{
	double distanceSum = 0.0, firstLen = 0.0, secondLen = 0.0;
	//std::cout<<"========="<<std::endl<<histOrig<<std::endl<<histTmp<<std::endl;
	Mat result = histOrig.mul(histTmp);
	sqrt(result, result);
	distanceSum = sum(result).val[0];
	return distanceSum;
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
	particleIter = ParticleList.begin();
	RNG rng;
	for(int i=0; i<particleNum; i++)
	{
		randNum = rng.uniform(0.0, sum);
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