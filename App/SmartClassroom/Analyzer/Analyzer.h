// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ANALYZER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ANALYZER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ANALYZER_EXPORTS
#define ANALYZER_API __declspec(dllexport)
#else
#define ANALYZER_API __declspec(dllimport)
#endif

#include <vector>
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

// This class is exported from the Analyzer.dll
class ANALYZER_API CPositionAnalyzer {
public:
	CPositionAnalyzer(void);
	HRESULT AnalyzeTeacherPositions(std::vector<Point2f> trackedPersons);
	// TODO: add your methods here.
};

