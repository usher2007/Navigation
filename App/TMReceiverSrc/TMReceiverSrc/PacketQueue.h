#pragma once
#include <streams.h>
extern "C" {
#include "libavutil/avstring.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/avassert.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
};

class PacketQueue {
public:
	PacketQueue();
	~PacketQueue();
	int Put(AVPacket *pkt);
	int Get(AVPacket *pkt, int block);
	int QueryFirst(AVPacket *pkt, int block);
	void Abort();
	void Flush();
	int size;
	int nb_packets;
private:
	AVPacketList *first_pkt;
	AVPacketList *last_pkt;


	int abort_request;
	CCritSec mutex;
	CAMEvent cond;
};//PacketQueueclass 