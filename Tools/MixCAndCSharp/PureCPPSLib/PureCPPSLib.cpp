#include "PureCPPSLib.h"

using namespace cv;
MyImage::MyImage(const string& imageName)
{
	_image = new DicomImage(imageName.c_str());
	if(_image != NULL)
	{
		if (_image->getStatus() == EIS_Normal)
		{
			if(_image->isMonochrome())
			{
				_image->setMinMaxWindow();
				char *pixelData = (char *)(_image->getOutputData(16,0,0));
				if (pixelData != NULL)
				{
					CvSize imgSize = CvSize(cvSize(_image->getHeight(), _image->getWidth()));
					int depth = _image->getDepth();
					int channels = _image->getFrameCount();
					IplImage* dst = new IplImage();
					dst = cvCreateImage(imgSize, depth, channels);
					dst->imageData = (char *)pixelData;
					dst->imageDataOrigin = (char *)pixelData;
					_img = Mat(dst);
				}
			}
		}
	}
}

void MyImage::ShowImage()
{
	namedWindow("lena", CV_WINDOW_AUTOSIZE);
	imshow("lena", _img);
	waitKey();
}