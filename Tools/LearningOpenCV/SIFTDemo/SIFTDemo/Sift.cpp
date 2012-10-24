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
	namedWindow("SIFT Demo", 0);
	if(argc == 1)
	{
		sprintf(fileName, "H:\\Example-Copy.jpg");
	}
	else
	{
		sprintf(fileName,"%s", argv[1]);
	}

	Mat img = imread(fileName);
	vector<KeyPoint> keypoints;
	Ptr<FeatureDetector> detector = FeatureDetector::create("SIFT");
	detector->detect(img, keypoints);
	vector<KeyPoint>::iterator kpIter = keypoints.begin();
	int i = 0;
	for(;kpIter != keypoints.end(); kpIter ++)
	{
		circle(img, kpIter->pt, 2, Scalar(0, 0, 255));
		i ++;
		if(kpIter->size < 60)
			break;
	}
	imshow("SIFT Demo", img);
	waitKey(0);
	imwrite("H:\\Example.bmp", img);
}
