#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include <sstream>
using namespace cv;

const string picDir = "H:\\GitHubCode\\Navigation\\Tools\\LearningOpenCV\\Data\\";
const Size brdSize(7, 7);
const Size imgSize(930, 623);
int main(void)
{
	//double A[4][2] = {{2,4},{3,-5},{1,2},{2,1}};
	//double B[4][1] = {{11},{3},{6},{7}};
	//Mat MatA = Mat(4,2,CV_64F, A);
	//Mat MatB = Mat(4,1,CV_64F, B);
	//Mat InvA = MatA.inv(DECOMP_SVD);
	//Mat res = InvA*MatB;
	//std::cout<<"Res:"<<(Mat_<double>&)res<<std::endl;
	vector<Mat> boards(8);
	for(int i=0; i<8; i++)
	{
		std::stringstream ss;
		ss << "pic" << (i+1) << ".jpg";
		string picFileName = ss.str();
		boards[i] = imread(picDir + picFileName, CV_LOAD_IMAGE_GRAYSCALE);
	}
	vector< vector<Point3f> > objectPoints1;
	vector< vector<Point3f> > objectPoints2;
	vector< vector<Point2f> > imagePoints1;
	vector< vector<Point2f> > imagePoints2;
	vector< Point2f > corners, cornersInFirstCamera, cornersInSecondCamera;
	vector< Point3f > chessBoard3D;
	for(int j=0; j<brdSize.height; ++j)
		for(int i=0; i<brdSize.width; ++i)
			chessBoard3D.push_back(Point3i(i, j, 0));

	for(size_t i=0; i<8; i++)
	{
		std::cout << i;
		namedWindow("Current Chessboard"); imshow("Current Chessboard", boards[i]); waitKey(100);
		bool found = findChessboardCorners(boards[i], brdSize, corners);
		if(found)
		{
			if(i==0)
			{
				findChessboardCorners(boards[i], brdSize, cornersInFirstCamera);
			}
			if(i==1)
			{
				findChessboardCorners(boards[i], brdSize, cornersInSecondCamera);
			}
			if(i%2==0)
			{
				imagePoints1.push_back(corners);
				objectPoints1.push_back(chessBoard3D);
			}
			else
			{
				imagePoints2.push_back(corners);
				objectPoints2.push_back(chessBoard3D);
			}
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

	Mat camMat1, camMat2;
	Mat distCoeffs1, distCoeffs2;
	vector<Mat> rvecs1, tvecs1, rvecs2, tvecs2;

	std::cout<<"Calibrating...";
	double rep_err1 = calibrateCamera(objectPoints1, imagePoints1, imgSize, camMat1, distCoeffs1, rvecs1, tvecs1);
	double rep_err2 = calibrateCamera(objectPoints2, imagePoints2, imgSize, camMat2, distCoeffs2, rvecs2, tvecs2);
	std::cout<<"Done"<<std::endl;

	std::cout<<"============================First Camera================================"<<std::endl;
	std::cout<<std::endl<<"Avg Reprojection error: " << rep_err1/4/brdSize.area() << std::endl;
	std::cout<<"Est camera matrix:\n"<<(Mat_<double>&)camMat1<<std::endl;
	std::cout<<"Est distCoeffs:\n"<< (Mat_<double>&)distCoeffs1<<std::endl;
	std::cout<<"Est 1st rvec:\n"<<(Mat_<double>&)rvecs1[0]<<std::endl;
	std::cout<<"Est 1st tvec:\n"<<(Mat_<double>&)tvecs1[0]<<std::endl;

	std::cout<<"============================Second Camera================================"<<std::endl;
	std::cout<<std::endl<<"Avg Reprojection error: " << rep_err2/4/brdSize.area() << std::endl;
	std::cout<<"Est camera matrix:\n"<<(Mat_<double>&)camMat2<<std::endl;
	std::cout<<"Est distCoeffs:\n"<< (Mat_<double>&)distCoeffs2<<std::endl;
	std::cout<<"Est 1st rvec:\n"<<(Mat_<double>&)rvecs2[0]<<std::endl;
	std::cout<<"Est 1st tvec:\n"<<(Mat_<double>&)tvecs2[0]<<std::endl;
	Mat R1,R2;
	Rodrigues(rvecs1[0], R1);
	Rodrigues(rvecs2[0], R2);
	for(int i=0; i<49; i++)
	{
		Mat origPointMat = Mat(objectPoints2[0][i]);
		Mat ThreeDInCamCoordinate = R2*(Mat_<double>)origPointMat;
		ThreeDInCamCoordinate += tvecs2[0];
		Mat TwoDInImage = camMat2 * ThreeDInCamCoordinate;
		std::cout<<"=============================Point: "<<i<<"============================\n";
		std::cout<<"Origin Point:\n"<<imagePoints2[0][i]<<std::endl;
		TwoDInImage = TwoDInImage / TwoDInImage.at<double>(2,0);
		std::cout<<"Est Point:\n"<<(Mat_<double>&)TwoDInImage<<std::endl;
	}

	Mat Trans1 = Mat(3,4,CV_64F);
	Mat Trans2 = Mat(3,4,CV_64F);
	Mat Ext1 = Mat(3,4,CV_64F);
	Mat Ext2 = Mat(3,4,CV_64F);
	for(int i=0; i<3; i++)
		for(int j=0; j<4; j++)
		{
			if(j==3)
			{
				Ext1.at<double>(i,j) = tvecs1[0].at<double>(i,0);
				Ext2.at<double>(i,j) = tvecs2[0].at<double>(i,0);
			}
			else
			{
				Ext1.at<double>(i,j) = R1.at<double>(i,j);
				Ext2.at<double>(i,j) = R2.at<double>(i,j);
			}
		}

	Trans1 = camMat1*Ext1;
	Trans2 = camMat2*Ext2;
	std::cout<<"%%%%%%%%%%%%%%%%%%%%%%Begin To Reconstruct...%%%%%%%%%%%%%%%%%%%%%\n";
	std::cout<<"1st R:\n"<<(Mat_<double>&)R1<<std::endl;
	std::cout<<"1st T:\n"<<(Mat_<double>&)tvecs1[0]<<std::endl;
	std::cout<<"1st Ext:\n"<<(Mat_<double>&)Ext1<<std::endl;
	std::cout<<"2nd R:\n"<<(Mat_<double>&)R2<<std::endl;
	std::cout<<"2nd T:\n"<<(Mat_<double>&)tvecs2[0]<<std::endl;
	std::cout<<"2nd Ext:\n"<<(Mat_<double>&)Ext2<<std::endl;
	for(int i=0; i<48; i++)
	{
		Point2f pInLeftCam = cornersInFirstCamera[i];
		Point2f pInRightCam = cornersInSecondCamera[i];
		Mat A = Mat(4,3,CV_64F);
		Mat B = Mat(4,1,CV_64F);
		A.at<double>(0,0) = pInLeftCam.x*Trans1.at<double>(2,0) - Trans1.at<double>(0,0);
		A.at<double>(0,1) = pInLeftCam.x*Trans1.at<double>(2,1) - Trans1.at<double>(0,1);
		A.at<double>(0,2) = pInLeftCam.x*Trans1.at<double>(2,2) - Trans1.at<double>(0,2);

		A.at<double>(1,0) = pInLeftCam.y*Trans1.at<double>(2,0) - Trans1.at<double>(1,0);
		A.at<double>(1,1) = pInLeftCam.y*Trans1.at<double>(2,1) - Trans1.at<double>(1,1);
		A.at<double>(1,2) = pInLeftCam.y*Trans1.at<double>(2,2) - Trans1.at<double>(1,2);

		A.at<double>(2,0) = pInRightCam.x*Trans2.at<double>(2,0) - Trans2.at<double>(0,0);
		A.at<double>(2,1) = pInRightCam.x*Trans2.at<double>(2,1) - Trans2.at<double>(0,1);
		A.at<double>(2,2) = pInRightCam.x*Trans2.at<double>(2,2) - Trans2.at<double>(0,2);
	
		A.at<double>(3,0) = pInRightCam.y*Trans2.at<double>(2,0) - Trans2.at<double>(1,0);
		A.at<double>(3,1) = pInRightCam.y*Trans2.at<double>(2,1) - Trans2.at<double>(1,1);
		A.at<double>(3,2) = pInRightCam.y*Trans2.at<double>(2,2) - Trans2.at<double>(1,2);

		B.at<double>(0,0) = Trans1.at<double>(0,3) - pInLeftCam.x*Trans1.at<double>(2,3);
		B.at<double>(1,0) = Trans1.at<double>(1,3) - pInLeftCam.y*Trans1.at<double>(2,3);
		B.at<double>(2,0) = Trans2.at<double>(0,3) - pInRightCam.x*Trans2.at<double>(2,3);
		B.at<double>(3,0) = Trans2.at<double>(1,3) - pInRightCam.y*Trans2.at<double>(2,3);
		Mat result3D = A.inv(DECOMP_SVD)*B;
		std::cout<<"Reconstruct 3D coordinate:\n"<<(Mat_<double>&)result3D<<std::endl;
	}
	return 0;
}