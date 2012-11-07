#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
using namespace cv;

class CameraCalib
{
public:
	int init(Size imgSize, Size brdSize);
	int Calibrate(vector<Mat> boards);
	Point2f GetImagePoint(int indexOfView, int indexOfPointInView) {return imagePoints[indexOfView][indexOfPointInView];}
	Mat GetTransMat(int indexOfView){return transM[indexOfView];}
	int PrintInfo(int indexOfView);
private:
	vector< vector<Point3f> > objectPoints;
	vector< vector<Point2f> > imagePoints;
	Size imageSize;
	Size boardSize;
	Mat camMat;
	Mat distCoeffs;
	vector<Mat> rvecs;
	vector<Mat> tvecs;
	vector<Mat> rotateM;
	vector<Mat> extM;
	vector<Mat> transM;
	vector< Point3f > chessBoard3D;
};