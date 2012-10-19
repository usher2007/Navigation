#pragma once
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace cv;
const string videoFileName = "H:\\GitHubCode\\Navigation\\Research\\ParticleFiltering\\FromShunli\\Video\\MVI_2132.avi";
const int offsetFrame = 10;
const int particleNum = 10;
const int histBinNum = 6;
const double colorNoise = 0.20;
const int stateNum = 4;
const int xNoise = 5;
const int yNoise = 2;
const int widthNoise = 0;
const int heightNoise = 0;
const double noiseWeight = 0.05;
const double PI=3.1415926;
Mat image;
int trackObject = 0;
bool selectObject = false;
Rect selection;
Point origin;
int hbins = 8, sbins = 8, vbins = 4;
int histSize[] = {hbins, sbins, vbins};
float hranges[] = {0, 256};
float sranges[] = {0, 256};
float vranges[] = {0, 256};
const float* phranges[] = {hranges, sranges, vranges};
class Particle
{
public:
	Particle(){xPos = yPos = width = height = 0; weight = 0.0;}
	Particle(int x, int y, int w, int h):xPos(x), yPos(y), width(w), height(h){ weight = 0.0; }
	int xPos, yPos, width, height;
	double weight;
};

class ParticleGroup
{
public:
	std::vector<Particle> ParticleList;
	int resampleParticle();
	int normalize();
	int generateParticleList(const Particle &originParticle, int num);
};
class ParticleTrackingAlg
{
public:
	ParticleTrackingAlg(){}
	int ProcessNextFrame(Mat &img);
	int calcParticleFeature(Mat &img, Particle p);
	int resampleParticle();
	double calcDistance(const Mat &histOrig, const Mat &histTmp);
	Mat hist, originHist;
	Mat roi;
	ParticleGroup particleGroup;
	Particle originParticle;
	std::vector<double> particlePdf;
};

