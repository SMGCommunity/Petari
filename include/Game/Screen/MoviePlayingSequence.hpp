#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/MovieSubtitles.hpp"
#include "Game/Util/Array.hpp"

class DemoPadRumbler;
struct MoviePlayingInfo;

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

    void exePlayWait();
    void exePlayStart();
    void exePlay();
    bool tryStartSubtitles();
    bool tryEnd();
    bool trySkip();
    void exeCloseWipeOnPlaying();
    void exeEndWait();

    MoviePlayingInfo* mInfo;                                            // 0x20
    MR::Vector<MR::AssignableArray<MovieSubtitles*> > mSubtitles;       // 0x24
    DemoPadRumbler* mPadRumbler;                                        // 0x30
};
