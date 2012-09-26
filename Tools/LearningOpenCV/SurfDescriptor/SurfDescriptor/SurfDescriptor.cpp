#include <iostream>
#include "opencv2\core\core.hpp"
#include "opencv2\features2d\features2d.hpp"
#include "opencv2\highgui\highgui.hpp"

using namespace cv;

int main(int argc, char **argv)
{
	Mat img_1 = imread("H:\\GitHubCode\\OpenCV\\opencv_extra\\learning_opencv_v2\\birdseye\\IMG_0214.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_2 = imread("H:\\GitHubCode\\OpenCV\\opencv_extra\\learning_opencv_v2\\birdseye\\IMG_0214L.jpg", CV_LOAD_IMAGE_GRAYSCALE);

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

	BruteForceMatcher<L2<float>> matcher;
	std::vector<DMatch> matches;
	matcher.match(descriptors_1, descriptors_2, matches);

	Mat img_matches;
	drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, img_matches);

	imwrite("H:\\example.jpg", img_matches);
	return 0;
}