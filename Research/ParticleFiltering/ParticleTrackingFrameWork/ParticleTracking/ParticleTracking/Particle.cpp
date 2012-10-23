#include "Particle.h"

Particle::Particle(const int xPos, const int yPos, const int width, const int height)
{
	particleRegion.x = xPos;
	particleRegion.y = yPos;
	particleRegion.width = width;
	particleRegion.height = height;
	this->weight = 0.0;
}

Mat Particle::GetParticleRoi(const Mat& image)
{
	particleRegion &= Rect(0, 0, image.cols, image.rows);
	Mat roi(image, particleRegion);
	return roi;
}

inline double Particle::GetParticleWeight()
{
	return weight;
}

inline int Particle::SetParticleWeight(double weight)
{
	this->weight = weight;
	return 0;
}