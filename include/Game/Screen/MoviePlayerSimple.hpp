#pragma once

#include <JSystem/JKernel/JKRDisposer.hpp>
#include "Game/Screen/LayoutActor.hpp"
#include <revolution/thp.h>

class THPSimplePlayerWrapper;
class JKRExpHeap;

struct Movie {
    const char* mMovieName;         // 0x00
    THPVideoInfo mVideoInfo;        // 0x04
    u8* mBuffer;                    // 0x10
    s32 mCurrentFrame;              // 0x14
    u8 _18;
    u8 _19;
    u32 mFrameRateDefault;          // 0x1C
    u32 _20;
    u32 _24;
};

class MoviePlayerSimple : public LayoutActor, public JKRDisposer {
public:
    MoviePlayerSimple();

    virtual ~MoviePlayerSimple();
    virtual void init(const JMapInfoIter &);
    virtual void draw() const;
    virtual void control();

    void startMovie(const char *, bool);
    void stopMovie();
    bool isMovieActive() const;
    bool isMoviePlaying() const;
    s32 getCurrentFrame() const;
    s32 getTotalFrame() const;
    void setFrameRateDefault();
    void setVolume(f32, s32);
    void setUnpauseHomeButtonFlag();
    void exeOpen();
    void exePreload();
    void exePlaying();
    inline void exeSimpleOff();
    inline void exeSimpleSuspend();
    void play(bool);
    void stop();
    u32 calcNeedMemoryForMovieWorks();
    void drawCinemaFrame() const;

    Movie* mMovie;                              // 0x38
    JKRExpHeap* mHeap;                          // 0x3C
    THPSimplePlayerWrapper* mPlayerWrapper;     // 0x40
    u8 _44;
    u8 _45;
};
