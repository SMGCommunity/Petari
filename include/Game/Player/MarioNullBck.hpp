#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MarioNullBck : public LiveActor {
public:
    MarioNullBck(const char*);

    virtual ~MarioNullBck();

    virtual void init(const JMapInfoIter&);
    virtual void appear();

    void getLastPos(TVec3f*);
    bool getFramePos(f32, TVec3f*, TVec3f*);

    u16 _8C;
};
