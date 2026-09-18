#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include <JSystem/JKernel/JKRDisposer.hpp>
#include <revolution/thp.h>

class THPSimplePlayerWrapper;
class JKRExpHeap;

struct Movie {
    /* 0x00 */ const char* mMovieName;
    /* 0x04 */ THPVideoInfo mVideoInfo;
    /* 0x10 */ u8* mBuffer;
    /* 0x14 */ u32 mCurrentFrame;
    /* 0x18 */ bool _18;
    /* 0x19 */ bool _19;
    /* 0x1C */ s32 mFrameRateDefault;
    /* 0x20 */ s32 _20;
    /* 0x24 */ u32 _24;
};

class MoviePlayerSimple : public LayoutActor, public JKRDisposer {
public:
    /// @brief Creates a new `MoviePlayerSimple`.
    MoviePlayerSimple();

    /// @brief Destroys the `MoviePlayerSimple`.
    virtual ~MoviePlayerSimple();

    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void control();

    void startMovie(const char*, bool);
    void stopMovie();
    bool isMovieActive() const;
    bool isMoviePlaying() const;
    s32 getCurrentFrame() const;
    s32 getTotalFrame() const;
    void setFrameRateDefault();
    void setVolume(f32, s32);
    void setUnpauseHomeButtonFlag();
    void exeSimpleOff();
    void exeOpen();
    void exePreload();
    void exePlaying();
    void exeSimpleSuspend();
    void play(bool);
    void stop();
    u32 calcNeedMemoryForMovieWorks();
    void drawCinemaFrame() const;

public:
    /* 0x38 */ Movie* mMovie;
    /* 0x3C */ JKRExpHeap* mHeap;
    /* 0x40 */ THPSimplePlayerWrapper* mPlayerWrapper;
    /* 0x44 */ bool _44;
    /* 0x44 */ bool _45;
};
