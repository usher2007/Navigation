#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace cv;

const string picDir = "H:\\GitHubCode\\Navigation\\Tools\\LearningOpenCV\\Data\\";
const string picFileOne = "pic1.jpg";
const string picFileTwo = "pic2.jpg";
const string picFileThree = "pic3.jpg";
const string picFileFour = "pic4.jpg";
const Size brdSize(7, 7);
const Size imgSize(930, 623);
int main(void)
{
	Mat picOne = imread(picDir + picFileOne, CV_LOAD_IMAGE_GRAYSCALE);
	Mat picTwo = imread(picDir + picFileTwo, CV_LOAD_IMAGE_GRAYSCALE);
	Mat picThree = imread(picDir + picFileThree, CV_LOAD_IMAGE_GRAYSCALE);
	Mat picFour = imread(picDir + picFileFour, CV_LOAD_IMAGE_GRAYSCALE);
	vector<Mat> boards(4);
	boards[0] = picOne;
	boards[1] = picTwo;
	boards[2] = picThree;
	boards[3] = picFour;
	vector< vector<Point3f> > objectPoints;
	vector< vector<Point2f> > imagePoints;
	vector< Point2f > corners;
	vector< Point3f > chessBoard3D;
	for(int j=0; j<brdSize.height; ++j)
		for(int i=0; i<brdSize.width; ++i)
			chessBoard3D.push_back(Point3i(i, j, 0));

	for(size_t i=0; i<4; i++)
	{
		std::cout << i;
		namedWindow("Current Chessboard"); imshow("Current Chessboard", boards[i]); waitKey(100);
		bool found = findChessboardCorners(boards[i], brdSize, corners);
		if(found)
		{
			imagePoints.push_back(corners);
			objectPoints.push_back(chessBoard3D);
			std::cout<< "-found";
		}
		else
		{
			std::cout<< "-not-found";
		}
		drawChessboardCorners(boards[i], brdSize, Mat(corners), found);
		imshow("Current Chessboard", boards[i]); waitKey(1000);
	}
	std::cout<<"Done"<<std::endl;

	Mat camMat;
	Mat distCoeffs;
	vector<Mat> rvecs, tvecs;

	std::cout<<"Calibrating...";
	double rep_err = calibrateCamera(objectPoints, imagePoints, imgSize, camMat, distCoeffs, rvecs, tvecs);
	std::cout<<"Done"<<std::endl;
}