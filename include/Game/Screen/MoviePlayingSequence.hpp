#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/Array.hpp"

class DemoPadRumbler;
class MovieSubtitles;

struct MoviePlayingInfo {
    const char* mMovieName;             // 0x00
    const char* mMovieNameLuigi;        // 0x04
    const char* mGalaxyName;            // 0x08
    const char* mMusic;                 // 0x0C
    s32 mPlayWaitTime;
    s32 _14;
    s32 _18;
    s32 _1C;
    s32 _20;
    s32 _24;
    s32 _28;
    f32 mVolume;                // 0x2C
};

class MoviePlayingSequence : public LayoutActor {
public:
    enum MovieType {
        MovieType_PrologueA,
        MovieType_PrologueB,
        MovieType_FinalBattle,
        MovieType_EpilogueA,
        Unk_4,
        MovieType_EndingA,
        MovieType_EndingB
    };

    MoviePlayingSequence(const char *, s32);

    virtual ~MoviePlayingSequence();
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

    const MoviePlayingInfo* mInfo;                                      // 0x20
    MR::Vector<MR::AssignableArray<MovieSubtitles*> > mSubtitles;       // 0x24
    DemoPadRumbler* mPadRumbler;                                        // 0x30
};

class MoviePlayingSequenceHolder : public NameObj {
public:
    MoviePlayingSequenceHolder(const char *);

    virtual ~MoviePlayingSequenceHolder();

    MR::AssignableArray<MoviePlayingSequence*> mSequences;          // 0xC
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
