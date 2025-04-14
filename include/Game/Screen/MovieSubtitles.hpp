#pragma once

#include "Game/Screen/LayoutActor.hpp"

class TalkTextFormer;

class MovieSubtitles : public LayoutActor {
public:
    MovieSubtitles(const char *, s32);

    virtual ~MovieSubtitles();
    virtual void appear();

    inline void exeWait();
    void exeTalk();
    void exeTalkWait();

    TalkTextFormer* mFormerText;            // 0x20
    s32 _24;
};
