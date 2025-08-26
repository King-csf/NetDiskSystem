#ifndef PACKETQUEUE_H
#define PACKETQUEUE_H

#include <queue>
#include  <mutex>
#include <condition_variable>
#include <QDebug>
extern "C"
{
#include "SDL3/SDL.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libavfilter/avfilter.h"
}



class PacketQueue
{
public:
    PacketQueue();
    std::queue<AVPacket> pktQueue;
    int maxNum;
    std::mutex mtx;

    std::condition_variable cv;
    bool isStop;
    void pushPkt(AVPacket& pkt);
    bool popPkt(AVPacket& pkt);
    //清理队列
    void cleanQueue();

};

class FrameQueue
{
public:
    FrameQueue();
    std::queue<AVFrame *> frameQueue;
    int maxNum;
    std::mutex mtx;
    bool isStop;
    std::condition_variable cv;

    void pushFrame(AVFrame * frame);
    AVFrame* popFrame();
    //清理队列
    void cleanQueue();
};

#endif // PACKETQUEUE_H
