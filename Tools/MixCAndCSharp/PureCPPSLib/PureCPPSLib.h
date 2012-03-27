#ifndef PURECPPSLIB_H
#define PURECPPSLIB_H
#include <string>
#include "config\osconfig.h"
#include "dcmdata\dctk.h"
#include "dcmimage\diregist.h"
#include "dcmimgle\dcmimage.h"
#include "cv.h"
#include "highgui.h"
using namespace std;
using namespace cv;
class MyImage
{
public:
	MyImage(const string& imageName);
	void ShowImage();
private:
	Mat _img;
	DicomImage* _image;
};
#endif

