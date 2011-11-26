#include <dcmtk\config\osconfig.h>
#include <dcmtk\dcmdata\dctk.h>

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
	return 0;
}