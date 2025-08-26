#include "packetqueue.h"

PacketQueue::PacketQueue()
{
    maxNum = 4096;
}

void PacketQueue::cleanQueue()
{
    std::unique_lock<std::mutex> lock(mtx);

    //释放数据
    while(!pktQueue.empty())
    {
        av_packet_unref(&pktQueue.front());
        pktQueue.pop();
    }
}

void PacketQueue::pushPkt(AVPacket& pkt)
{
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock,[&]{
        return pktQueue.size() < maxNum || isStop;
    });
    if(isStop)
    {
        return ;
    }
    AVPacket temp;
    av_packet_move_ref(&temp,&pkt);
    pktQueue.push(temp);
    cv.notify_one();

}
bool PacketQueue::popPkt(AVPacket& pkt)
{
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock,[&]{
        return !pktQueue.empty() || isStop;
    });
    if(isStop)
    {
        return false;
    }
    //因停止被唤醒，但队列为空退出
    if(pktQueue.empty() && isStop)
    {
        return false;
    }
    av_packet_move_ref(&pkt,&pktQueue.front());
    pktQueue.pop();
    cv.notify_one();
    return true;
}


FrameQueue::FrameQueue()
{

    maxNum = 50;
}

void FrameQueue::cleanQueue()
{
    std::unique_lock<std::mutex> lock(mtx);

    while(!frameQueue.empty())
    {
        AVFrame* frame = frameQueue.front();
        frameQueue.pop();
        av_frame_free(&frame);
    }

}

void FrameQueue::pushFrame(AVFrame * frame)
{
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock,[&]{
        return frameQueue.size() < maxNum || isStop;
    });
    if (isStop)
    {
        qDebug() << "pushFrame stop";
        av_frame_free(&frame);
        return;
    }

    frameQueue.push(frame);
    cv.notify_one();

}
AVFrame* FrameQueue::popFrame()
{
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock,[&]{
        return !frameQueue.empty() || isStop;
    });
    if(isStop)
    {
        qDebug() << "popFrame stop";
        return NULL;
    }
    if(isStop && frameQueue.empty())
    {
        return NULL;
    }
    AVFrame* frame = frameQueue.front();
    frameQueue.pop();
    cv.notify_one();
    return frame;
}
