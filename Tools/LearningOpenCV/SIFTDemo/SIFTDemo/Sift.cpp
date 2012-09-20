#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"

#include <limits>
#include <cstdio>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	char fileName[1024];

	if(argc == 1)
	{
		sprintf(fileName, "H:\\Example.jpg");
	}
	else
	{
		sprintf(fileName,"%s", argv[1]);
	}

	Mat img = imread(fileName);
	vector<KeyPoint> keypoints;
	Ptr<FeatureDetector> detector = FeatureDetector::create("SIFT");
	detector->detect(img, keypoints);
}
