#include "config\osconfig.h"
#include "dcmdata\dctk.h"
#include "dcmimage\diregist.h"
#include "dcmimgle\dcmimage.h"
#include "cv.h"
#include "highgui.h"
using namespace cv;
int main(void)
{
	DcmFileFormat fileformat;
	OFCondition oc = fileformat.loadFile("D:\\021A3C1C.dcm");
	if(oc.good())
	{
		OFString patientName;
		if(fileformat.getDataset()->findAndGetOFString(DCM_PatientName, patientName).good())
		{
			std::cout<<"Patient Name:"<<patientName<<std::endl;
		}
	}
	DicomImage *image = new DicomImage("D:\\021A3C1C.dcm");
	if (image != NULL)
	{
		if (image->getStatus() == EIS_Normal)
		{
			if (image->isMonochrome())
			{
				image->setMinMaxWindow();
				char *pixelData = (char *)(image->getOutputData(16,0,0));
				if(pixelData != NULL)
				{
					/*do something */
					CvSize imgSize = CvSize(cvSize(image->getHeight(),image->getWidth()));
					int depth = image->getDepth();
					int channels = image->getFrameCount();
					IplImage* dst = new IplImage();
					dst = cvCreateImage(imgSize, depth, channels);
					(dst)->imageData = (char *)pixelData;
					(dst)->imageDataOrigin = (char *)pixelData;
					Mat img(dst);
					namedWindow("lena", CV_WINDOW_AUTOSIZE);
					cvShowImage("lena", dst);
					waitKey();
					return 0;
				}
			}
			else
			{
				std::cerr<< "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << std::endl;
			}
		}
	}
	return 0;
}

