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