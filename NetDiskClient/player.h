#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include "packetqueue.h"
#include <QString>
#include <QDebug>
#include <atomic>
#include <chrono>
#include <QTimer>
#include <thread>
extern "C"
{
#include "SDL3/SDL.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libavutil/error.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}

namespace Ui {
class Player;
}

class Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = nullptr);
    explicit Player(QString fileName,QWidget *parent = nullptr);
    ~Player();

    //播放的文件
    QString filename;

    SDL_Window * sdlWindow; //窗口
    SDL_Renderer * render;  //渲染器
    SDL_Texture * texture;  //纹理

    AVRational sar; //视频宽高比

    PacketQueue audioPkt;   //音频pkt队列
    PacketQueue videoPkt;   //视频pkt队列
    FrameQueue  audioFrame;
    FrameQueue  videoFrame;
    //输入上下文
    AVFormatContext * inCtx;

    int vIdx;
    int aIdx;

    //线程是否停止
    std::atomic<bool> isStop;
    //相关操作是否结束
    std::atomic<bool> isDemuxer;
    std::atomic<bool> isAuDecode;
    std::atomic<bool> isViDecode;

    double totalTime;

    //音频解码
    //const AVCodec * auCodec;
    AVCodecContext *auCodeCtx;
    //视频解码
    //const AVCodec * viCodec;
    AVCodecContext *viCodeCtx;

    AVFilterGraph * graph;
    AVFilterContext * srcBuffer;
    AVFilterContext * sinkBuffer;
    AVFilterContext * speedFilter;
    AVFilterContext * volumeFilter;

    enum threadType{
        THREAD_DEMUXER = 0,
        THREAD_AUDIO_DECODE,
        THREAD_VIDEO_DECODE,
        THREAD_DELAY_VIDEO,
        THREAD_PLAY_AUDIO
    };

    //获取单例
    static Player* getInstance();
    //线程运行函数
    static void threadFun(threadType flag);
    //解封装
    int demuxer(const QString& filename);
    int videoDeocode();
    int audioDecode();
    void delayVideo();
    void playAudio();
    void destory();
    AVFrame *  useSpeedFilter(AVFrame * frame);
    bool initSpeedFilter();

    double audioClock;
    double videoClock;
    double currentAudioPts; // 当前音频帧的PTS，用于正确计算音频时钟

    float speed; //倍速
    std::atomic<bool> isModSpeed;
    SwrContext *swrCtx;
    AVFrame * speedFrame;
    SDL_AudioStream * stream; // SDL音频流

    //是否快进
    std::atomic<bool> isJump;
    //跳转到的时间
    int jumpSec;
    std::atomic<bool> isJumpAuDecode;
    std::atomic<bool> isJumpViDecode;
    std::atomic<bool> isPause;
    std::atomic<bool> isJumpInitFilter;

    std::atomic<bool> isModVolume;
    std::atomic<float> volume;
    SDL_AudioFormat ToSDLFormat(enum AVSampleFormat format);

    QTimer* timer;

    double curSec;

    void setSDLWindow();
    void start();

    std::thread muxerThread;
    std::thread viDecoderThread;
    std::thread auDecoderThread;
    std::thread delayVideoThread;
    std::thread playAudioThread;

    void initProgressBar(); //初始化进度条
    void initSoundBar(); //初始化音量条

private slots:
    void on_stop_btn_clicked();

    void on_start_btn_clicked();

    void on_next_btn_clicked();

    void on_last_btn_clicked();

    void on_progress_bar_sliderPressed();

    void on_speedComboBox_currentIndexChanged(int index);

    void on_sound_bar_sliderPressed();

private:
    Ui::Player *ui;
};

#endif // PLAYER_H
