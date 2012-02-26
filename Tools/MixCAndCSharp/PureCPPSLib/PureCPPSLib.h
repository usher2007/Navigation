#include "cv.h"
#include "highgui.h"
#include <string>
using namespace std;
using namespace cv;
class MyImage
{
public:
	MyImage(const string& imageName);
	void ShowImage();
private:
	Mat _img;
};

