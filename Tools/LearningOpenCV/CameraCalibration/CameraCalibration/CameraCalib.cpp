#include "CameraCalib.h"

int CameraCalib::init(Size imgSize, Size brdSize)
{
	imageSize = imgSize;
	boardSize = brdSize;
	for(int j=0; j<boardSize.height; j++)
		for(int i=0; i<boardSize.width; i++)
		{
			chessBoard3D.push_back(Point3i(i,j,0));
		}
	return 0;
}

int CameraCalib::Calibrate(vector<Mat> boards)
{
	vector< Point2f > corners;
	for(int i=0; i<boards.size(); i++)
	{
		bool found = findChessboardCorners(boards[i], boardSize, corners);
		if(found)
		{
			imagePoints.push_back(corners);
			objectPoints.push_back(chessBoard3D);
		}
	}
	if(imagePoints.size() < 2)
	{
		return -1;
	}
	double rep_err = calibrateCamera(objectPoints, imagePoints, imageSize, camMat, distCoeffs, rvecs, tvecs);
	for(int i=0; i<rvecs.size(); i++)
	{
		Mat R;
		Mat Trans = Mat(3,4, CV_64F);
		Mat Ext = Mat(3,4, CV_64F);
		Rodrigues(rvecs[i], R);
		rotateM.push_back(R);
		for(int j=0; j<3; j++)
			for(int k=0; k<4; k++)
			{
				if(k==3)
				{
					Ext.at<double>(j,k) = tvecs[i].at<double>(j,0);
				}
				else
				{
					Ext.at<double>(j,k) = R.at<double>(j,k);
				}
			}
		extM.push_back(Ext);
		Trans = camMat * Ext;
		transM.push_back(Trans);
	}
	return 0;
}

int CameraCalib::PrintInfo(int indexOfView)
{
	std::cout << "=========================Camera Calibrate Result=========================\n";
	std::cout << "Camera Mat:\n"<<(Mat_<double>&)camMat<<std::endl;
	std::cout << "Distort Coeffs:\n"<< (Mat_<double>&)distCoeffs<<std::endl;
	if(indexOfView < 0) return 0;
	std::cout << "\nFor " <<indexOfView<<" View:\n\n";
	std::cout << "External Mat:\n"<<(Mat_<double>&)extM[indexOfView]<<std::endl;
	std::cout << "Rotate Mat:\n"<<(Mat_<double>&)transM[indexOfView]<<std::endl;
	return 0;
}