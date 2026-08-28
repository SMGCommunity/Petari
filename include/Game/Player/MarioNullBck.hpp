#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MarioNullBck : public LiveActor {
public:
    MarioNullBck(const char*);

    /* 0x08 */ virtual ~MarioNullBck();

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();

    void getLastPos(TVec3f*);
    bool getFramePos(f32, TVec3f*, TVec3f*);

    u16 _8C;
};
