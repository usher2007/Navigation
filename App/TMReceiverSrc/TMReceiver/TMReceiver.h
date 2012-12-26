// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TMRECEIVER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TMRECEIVER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#ifdef TMRECEIVER_EXPORTS
#define TMRECEIVER_API __declspec(dllexport)
#else
#define TMRECEIVER_API __declspec(dllimport)
#endif

// This class is exported from the TMReceiver.dll
class TMRECEIVER_API CTMReceiver {
public:
	CTMReceiver(void);
	// TODO: add your methods here.
};

extern TMRECEIVER_API int nTMReceiver;

TMRECEIVER_API int fnTMReceiver(void);

//基本直播功能

typedef struct TMPicture_t{

	unsigned char *data[8];

	int linesize[8];

} TMPicture;

typedef struct TMFrame_t{

	TMPicture pic;//解码后的数据

	char* data; //指向视频编码数据段的指针

	int len; //视频编码数据长度

	int decoded; //是否已经解码

	int error;

}TMFrame;

typedef int (*TMReceiverCB)(TMFrame* pFrame, void* arg);//解码数据处理回调函数原型

typedef enum{

	STORAGE_FT_MP4,

	STORAGE_FT_TS,

	STORAGE_FT_FLV,

	STORAGE_FT_UNKNOWN

}TMStorageFileType;

typedef void* TMRecieverHandle;
typedef void* TMHandle;

TMRECEIVER_API TMRecieverHandle TM_RecieverCreate();
TMRECEIVER_API int TM_RecieverOpenRtspStream(TMRecieverHandle hReciever, TMHandle h, char* rtspUrl);
TMRECEIVER_API int TM_RecieverOpenHttpStream(TMRecieverHandle hReciever, TMHandle h, char* strFileName);
TMRECEIVER_API int TM_RecieverEnableDisplay(TMRecieverHandle hReciever, HWND hWnd);
TMRECEIVER_API int TM_RecieverDisableDisplay(TMRecieverHandle hReciever);
TMRECEIVER_API int TM_RecieverStartDisplay(TMRecieverHandle hReciever);
TMRECEIVER_API int TM_RecieverStopDisplay(TMRecieverHandle hReciever);
TMRECEIVER_API int TM_RecieverEnableStorage(TMRecieverHandle hReciever, const char* strFileName);
TMRECEIVER_API int TM_RecieverDisableStorage(TMRecieverHandle hReciever);
TMRECEIVER_API int TM_RecieverStartStorage(TMRecieverHandle hReciever);
TMRECEIVER_API int TM_RecieverStopStorage(TMRecieverHandle hReciever);
TMRECEIVER_API int TM_RecieverSetCallBackBeforeDecode(TMRecieverHandle hReciever, TMReceiverCB cb, void* arg);
TMRECEIVER_API int TM_RecieverSetCallBackAfterDecode(TMRecieverHandle hReciever, TMReceiverCB cb, void* arg);
TMRECEIVER_API int TM_RecieverClose(TMRecieverHandle hReciever);
