#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/Array.hpp"

class DemoPadRumbler;
class MovieSubtitles;

struct MoviePlayingInfo {
    /* 0x00 */ const char* mMovieName;
    /* 0x04 */ const char* mMovieNameLuigi;
    /* 0x08 */ const char* mStageName;
    /* 0x0C */ const char* mStageBgmName;
    /* 0x10 */ s32 mPlayWaitTime;
    /* 0x14 */ s32 mEndWaitTime;
    /* 0x18 */ s32 mOpenWipeType;
    /* 0x1C */ s32 mOpenWipeTime;
    /* 0x20 */ s32 mCloseWipeType;
    /* 0x24 */ s32 mCloseWipeTime;
    /* 0x28 */ s32 mEndWaitWipeType;
    /* 0x2C */ f32 mVolume;
};

class MoviePlayingSequence : public LayoutActor {
public:
    enum MovieType {
        MovieType_PrologueA,
        MovieType_PrologueB,
        MovieType_FinalBattle,
        MovieType_EpilogueA,
        MovieType_EpilogueB,
        MovieType_EndingA,
        MovieType_EndingB,
    };

    MoviePlayingSequence(const char *, s32);

    virtual void appear();
    virtual void kill();

    static const char* getMovieName(MovieType);

    void exeWait();
    void exePlayWait();
    void exePlayStart();
    void exePlay();
    bool tryStartSubtitles();
    bool tryEnd();
    bool trySkip();
    void exeCloseWipeOnPlaying();
    void exeEndWait();

private:
    /* 0x20 */ const MoviePlayingInfo* mInfo;
    /* 0x24 */ MR::Vector<MR::AssignableArray<MovieSubtitles*> > mSubtitles;
    /* 0x30 */ DemoPadRumbler* mPadRumbler;
};

class MoviePlayingSequenceHolder : public NameObj {
public:
    MoviePlayingSequenceHolder(const char *);

    /* 0x0C */ MR::AssignableArray<MoviePlayingSequence*> mSequence;
};

namespace MR {
    void createMoviePlayingSequence();
    void startMovie(int);
    bool isEndMovie(int);
    void startMovieEpilogueA();
    void startMovieEndingA();
    void startMovieEndingB();
    bool isEndMovieEpilogueA();
    bool isEndMovieEndingA();
    bool isEndMovieEndingB();
    bool isMoviePlayingOnSequence();
};
