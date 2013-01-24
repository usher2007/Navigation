// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the H264DEC_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// H264DEC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef H264DEC_EXPORTS
#define H264DEC_API __declspec(dllexport)
#else
#define H264DEC_API __declspec(dllimport)
#endif


extern H264DEC_API int nH264Dec;

H264DEC_API int fnH264Dec(void);

typedef void* H264Dec_Handle;

typedef struct Picture_t{
	unsigned char *data[4];
	int linesize[4];
} Picture;

H264DEC_API void H264Dec_Init();
H264DEC_API H264Dec_Handle H264Dec_Create();
H264DEC_API bool H264Dec_IsReady(H264Dec_Handle h);
H264DEC_API unsigned int H264Dec_GetWidth(H264Dec_Handle h);
H264DEC_API unsigned int H264Dec_GetHeight(H264Dec_Handle h);
H264DEC_API int H264Dec_DecodeFrame(H264Dec_Handle h, unsigned char* img, unsigned char* data, unsigned long size);
H264DEC_API void H264Dec_Delete(H264Dec_Handle h);
