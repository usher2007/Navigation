// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LOGGER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LOGGER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LOGGER_EXPORTS
#define LOGGER_API __declspec(dllexport)
#else
#define LOGGER_API __declspec(dllimport)
#endif

#include <iostream>


// This class is exported from the Logger.dll
class LOGGER_API CLogger {
public:
	CLogger(void);
	CLogger(const char* infoLogFile, const char* errorLogFile);

	int logInfo(const char* logStr);
	int logError(const char* logStr);
private:
	ofstream m_infoLogFile;
	ofstream m_errorLogFile;
};

