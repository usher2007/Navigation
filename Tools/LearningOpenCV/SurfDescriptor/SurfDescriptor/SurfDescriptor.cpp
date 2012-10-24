#include <iostream>
#include "opencv2\core\core.hpp"
#include "opencv2\features2d\features2d.hpp"
#include "opencv2\highgui\highgui.hpp"

using namespace cv;

int main(int argc, char **argv)
{
	//Mat img_1 = imread("H:\\GitHubCode\\OpenCV\\opencv_extra\\learning_opencv_v2\\birdseye\\IMG_0214.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//Mat img_2 = imread("H:\\GitHubCode\\OpenCV\\opencv_extra\\learning_opencv_v2\\birdseye\\IMG_0214L.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_1 = imread("H:\\Example1-Copy.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_2 = imread("H:\\Example2.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	if(!img_1.data || !img_2.data)
	{
		return -1;
	}

	int minHessian = 5000;
	SurfFeatureDetector detector(minHessian);
	std::vector<KeyPoint> keypoints_1, keypoints_2;

	detector.detect(img_1, keypoints_1);
	detector.detect(img_2, keypoints_2);

	SurfDescriptorExtractor extractor;

	Mat descriptors_1, descriptors_2;

	extractor.compute(img_1, keypoints_1, descriptors_1);
	extractor.compute(img_2, keypoints_2, descriptors_2);

	/*BruteForceMatcher<L2<float>> matcher;
	std::vector<DMatch> matches;
	matcher.match(descriptors_1, descriptors_2, matches);

	Mat img_matches;
	drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, img_matches);

	imwrite("H:\\example.jpg", img_matches);
	return 0;*/

	FlannBasedMatcher matcher;
	std::vector<DMatch> matches;
	matcher.match(descriptors_1, descriptors_2, matches);

	double max_dist = 0;
	double min_dist = 100;

	for(int i=0; i<descriptors_1.rows; i++)
	{
		double dist = matches[i].distance;
		if(dist < min_dist) min_dist = dist;
		if(dist > max_dist) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	std::vector<DMatch> good_matches;

	for(int i=0; i<descriptors_1.rows; i++)
	{
		if(matches[i].distance < 2*min_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}

	Mat img_matches;
	drawMatches(img_1, keypoints_1, img_2, keypoints_2, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	imshow("Good Matches", img_matches);
	
	for(int i=0; i<good_matches.size(); i++)
	{
		printf("-- Good Match [%d] Keypoint 1 : %d -- Keypoint 2 : %d \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx);
	}

	waitKey(0);
	imwrite("H:\\example.jpg", img_matches);
	return 0;
}