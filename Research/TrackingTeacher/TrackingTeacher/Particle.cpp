#include "Particle.h"

Particle::Particle(const int xPos, const int yPos, const int width, const int height)
{
	particleRegion.x = xPos;
	particleRegion.y = yPos;
	particleRegion.width = width;
	particleRegion.height = height;
	this->weight = 0.0;
}

Particle::Particle(const Rect& region, const double rndX /* = 0.0 */, const double rndY /* = 0.0 */)
{
	this->particleRegion = region;
	this->particleRegion.x += rndX;
	this->particleRegion.y += rndY;
}

Mat Particle::GetParticleRoi(const Mat& image)
{
	particleRegion &= Rect(0, 0, image.cols, image.rows);
	Mat roi(image, particleRegion);
	return roi;
}

double Particle::GetParticleWeight()
{
	return weight;
}
int Particle::SetParticleWeight(double weight)
{
	this->weight = weight;
	return 0;
}

const Rect& Particle::GetParticleRegion()
{
	return particleRegion;
}