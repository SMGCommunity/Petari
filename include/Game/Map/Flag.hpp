#pragma once

#include "Game/LiveActor/LiveActor.hpp"


class Flag : public LiveActor {
public:
    Flag(const char*);
    void setInfoPos(const char*, const TVec3f*, const TVec3f&, f32, f32, f32, s32, s32, f32);

    virtual ~Flag();
    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual void draw() const;
    virtual void appear();

    // Members go here...
    /* 0x8C */ u8 _8C[0xA8];

};
