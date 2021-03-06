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
		Mat tmpImage;
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
			image.copyTo(tmpImage);
			rectangle(tmpImage,selection, Scalar(0,0,0));
			imshow("Particle Demo", tmpImage);
		}
		else
		{
			imshow("Particle Demo", image);
		}
		waitKey(10);
	}

	trackingAlg.particleGroup.generateParticleList(trackingAlg.originParticle, particleNum);
	double particleWeight[particleNum];

	Rect lastObjectRegion;
	cap >> frame;
	frame.copyTo(image);
	//cvtColor(image, hsv, CV_BGR2HSV);
	lastObjectRegion.x = trackingAlg.originParticle.xPos;
	lastObjectRegion.y = trackingAlg.originParticle.yPos;
	lastObjectRegion.width = trackingAlg.originParticle.width;
	lastObjectRegion.height = trackingAlg.originParticle.height;
	lastObjectRegion &= Rect(0, 0, image.cols, image.rows);
	CalcHsvHist(trackingAlg.originHist, image, lastObjectRegion);
	int frameCount = 0;
	double dist = 0.0;
	double pdf = 0.0;
	long start = 0;
	long endTime = 0;
	long sumTime = 0;
	start = clock();
	for(;;)
	{
		cap >> frame;
		if(frame.empty())
		{
			break;
		}
		start = clock();
		frame.copyTo(image);
		//Mat hsv;
		//cvtColor(image, hsv, CV_BGR2HSV);
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
		imshow("Particle Demo", image);
		waitKey(2);
	}
	endTime = clock();
	sumTime += (endTime - start);
	std::cout<<"SumTime: "<<sumTime*1.0/CLOCKS_PER_SEC<<std::endl;
}

int CalcHsvHist(Mat &hist, Mat image, Rect &selectRoi)
{
	Mat hsv, mask;
	//cvtColor(image, hsv, CV_BGR2HSV);
	//Mat roi(hue, selectRoi), maskroi(mask, selectRoi);
	Mat roi(image, selectRoi);
	int channels[3] = {0, 1, 2};
	calcHist(&roi, 1, channels, Mat(), hist, 3, histSize, phranges);
	Scalar histSum = sum(hist);

	hist = hist / histSum.val[0];
	return 0;
}

double ParticleTrackingAlg::calcDistance(const Mat &histOrig, const Mat &histTmp)
{
	double distanceSum = 0.0, firstLen = 0.0, secondLen = 0.0;
	//std::cout<<"========="<<std::endl<<histOrig<<std::endl<<histTmp<<std::endl;//
	Mat result = histOrig.mul(histTmp);
	sqrt(result, result);
	distanceSum = sum(result).val[0];
	return distanceSum;
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