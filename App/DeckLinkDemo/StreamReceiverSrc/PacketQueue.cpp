#include "stdafx.h"
#include "PacketQueue.h"
static AVPacket flush_pkt;

PacketQueue::PacketQueue()
{	
	first_pkt = NULL;
	last_pkt = NULL;
	nb_packets = 0;
	size = 0;
	abort_request = 0;
	av_init_packet(&flush_pkt);
	flush_pkt.data = (unsigned char*)"FLUSH";
	Put(&flush_pkt);
}

PacketQueue::~PacketQueue()
{
	Flush();
}

void PacketQueue::Flush()
{
	AVPacketList *pkt, *pkt1;
	mutex.Lock();
	for(pkt = first_pkt; pkt != NULL; pkt = pkt1) {
		pkt1 = pkt->next;
		av_free_packet(&pkt->pkt);
		av_freep(&pkt);
	}
	last_pkt = NULL;
	first_pkt = NULL;
	nb_packets = 0;
	size = 0;
	mutex.Unlock();
}

void PacketQueue::Abort()
{
	mutex.Lock();
	abort_request = 1;
	cond.Set();
	mutex.Unlock();
}
int PacketQueue::Put(AVPacket *pkt)
{
	AVPacketList *pkt1;
	/* duplicate the packet */
	if (pkt!=&flush_pkt && av_dup_packet(pkt) < 0)
		return -1;
	pkt1 = (AVPacketList *)av_malloc(sizeof(AVPacketList));
	if (!pkt1)
		return -1;
	pkt1->pkt = *pkt;
	pkt1->next = NULL;

	mutex.Lock();
	if (!last_pkt)
		first_pkt = pkt1;
	else
		last_pkt->next = pkt1;
	last_pkt = pkt1;
	nb_packets++;
	size += pkt1->pkt.size + sizeof(*pkt1);

	cond.Set();
	mutex.Unlock();
	return 0;
}

int PacketQueue::Get(AVPacket *pkt, int block)
{
	AVPacketList *pkt1;
	int ret;
	mutex.Lock();

	for(;;) {
		if (abort_request) {
			ret = -1;
			break;
		}

		pkt1 = first_pkt;
		if (pkt1) {
			first_pkt = pkt1->next;
			if (!first_pkt)
				last_pkt = NULL;
			nb_packets--;
			size -= pkt1->pkt.size + sizeof(*pkt1);
			*pkt = pkt1->pkt;
			av_free(pkt1);
			ret = 1;
			break;
		} else if (!block) {
			ret = 0;
			break;
		} else {
			mutex.Unlock();
			cond.Wait();
			mutex.Lock();
		}
	}
	mutex.Unlock();
	return ret;
}

int PacketQueue::QueryFirst(AVPacket *pkt, int block)
{
	AVPacketList *pkt1;
	int ret;
	mutex.Lock();

	for(;;) {
		if (abort_request) {
			ret = -1;
			break;
		}

		pkt1 = first_pkt;
		if (pkt1) {
			*pkt = pkt1->pkt;
			ret = 1;
			break;
		} else if (!block) {
			ret = 0;
			break;
		} else {
			mutex.Unlock();
			cond.Wait();
			mutex.Lock();
		}
	}
	mutex.Unlock();
	return ret;
}