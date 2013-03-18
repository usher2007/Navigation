#include "ParticleTrackingAlg.h"

const char *resultDir = "D:\\Navigation\\Research\\ParticleFiltering\\FromShunli\\Result\\";

ParticleTrackingAlg::ParticleTrackingAlg(int particleNum)
{
	this->particleNum = particleNum;
}

int ParticleTrackingAlg::AddParticleFeature(ParticleFeature* feature)
{
	features.push_back(feature);
	return 0;
}

int ParticleTrackingAlg::SetInitialObject( const Rect& trackObject, const Mat& image )
{
	this->trackObject = trackObject;
	this->resultObject = trackObject;
	Mat roi(image, trackObject);
	vector<ParticleFeature *>::iterator featureIter;
	for(featureIter=features.begin(); featureIter!=features.end(); featureIter++)
	{
		(*featureIter)->GenerateOriginFeature(roi);
	}
	return 0;
}

int ParticleTrackingAlg::Tracking(VideoCapture cap)
{
	namedWindow( "Tracking Result", CV_WINDOW_AUTOSIZE );
	generateParticleList();
	double sumPdf;
	int i=0; 
	char* resultFile = new char[1024];
	for(;;)
	{
		cap >> frame;
		if(frame.empty())
		{
			break;
		}
		frame.copyTo(image);
		vector<Particle>::iterator particleIter;
		vector<ParticleFeature *>::iterator featureIter;
		sumPdf = 0;
		for(particleIter=particleList.begin(); particleIter!=particleList.end(); particleIter++)
		{
			Mat particleRoi = particleIter->GetParticleRoi(image);
			double particleWeight = 0;
			for(featureIter=features.begin(); featureIter!=features.end(); featureIter++)
			{
				double pdf = ((*featureIter)->CalcSimilarityToOrigin(particleRoi)) * ((*featureIter)->GetFeatureWeight());
				//For multi-feature, the sumPdf should be specified to one feature.
				sumPdf += pdf;
				particleWeight += pdf;
			}
			particleIter->SetParticleWeight(particleWeight);
		}
		for(particleIter=particleList.begin(); particleIter!=particleList.end(); particleIter++)
		{
			double weight = particleIter->GetParticleWeight() / sumPdf;
			particleIter->SetParticleWeight(weight);
		}

		double newX = 0.0, newY = 0.0;
		for(particleIter=particleList.begin(); particleIter!=particleList.end(); particleIter++)
		{
			newX += particleIter->GetParticleWeight() * particleIter->GetParticleRegion().x;
			newY += particleIter->GetParticleWeight() * particleIter->GetParticleRegion().y;
		}
		resultObject.x = newX;
		resultObject.y = newY;
		rectangle(image,resultObject, Scalar(0,0,0));
		imshow("Tracking Result", image);
		waitKey(2);
		memset(resultFile, 0x00, 1024);
		sprintf(resultFile, "%s%d.bmp", resultDir, i++);
		imwrite(resultFile, image);
		resampleParticleList();
	}
	return 0;
}


//private methods
int ParticleTrackingAlg::generateParticleList()
{
	RNG rng;
	for(int i=0; i<particleNum; i++)
	{
		Particle p(trackObject.x + rng.gaussian(Utility::xNoise), trackObject.y + rng.gaussian(Utility::yNoise), trackObject.width, trackObject.height);
		particleList.push_back(p);
	}
	return 0;
}

int ParticleTrackingAlg::resampleParticleList()
{
	double *cumPdf = new double[particleNum];
	std::vector<Particle> newParticleList;

	std::vector<Particle>::iterator particleIter;
	double sumWeight = 0.0;
	int index = 0;
	for(particleIter=particleList.begin(); particleIter!=particleList.end(); ++particleIter)
	{
		sumWeight += particleIter->GetParticleWeight();
		cumPdf[index] = sumWeight;
		index++;
	}

	double randNum = 0.0;
	particleIter = particleList.begin();
	RNG rng;
	for(int i=0; i<particleNum; i++)
	{
		randNum = rng.uniform(0.0, sumWeight);
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
		Particle p((particleIter+j)->GetParticleRegion(), rng.gaussian(Utility::xNoise), rng.gaussian(Utility::yNoise));
		p.SetParticleWeight((particleIter+j)->GetParticleWeight());
		newParticleList.push_back(p);
	}
	particleList = newParticleList;
	return 0;
}

