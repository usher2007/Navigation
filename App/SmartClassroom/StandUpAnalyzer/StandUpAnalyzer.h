// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the STANDUPANALYZER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// STANDUPANALYZER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef STANDUPANALYZER_EXPORTS
#define STANDUPANALYZER_API __declspec(dllexport)
#else
#define STANDUPANALYZER_API __declspec(dllimport)
#endif

#include <iostream>
#include <map>


typedef struct _standUpInfo
{
	double slope;
	double weight;
	int pos;
} StandUpInfo;

typedef std::map<int, StandUpInfo> StandUpRowInfo;
typedef std::map<int, StandUpInfo>::iterator StandUpInfoIter;

// This class is exported from the StandUpAnalyzer.dll
class STANDUPANALYZER_API CStandUpAnalyzer {
public:
	CStandUpAnalyzer(void);
	// TODO: add your methods here.
	HRESULT AnalyzePosition(int index, StandUpRowInfo info);

private:
	HRESULT controlPTZCamera();

private:
	void* m_pModulerFactory;
	StandUpRowInfo m_StandUpInfo[2];
	int m_standUpStatus[9];    // 1 for standup, -1 for sitdown
};
