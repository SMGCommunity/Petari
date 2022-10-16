#pragma once

#include "Game/LiveActor/LiveActor.h"

class LargeChainParts : public LiveActor {
public:
    LargeChainParts(const char *pName = "でかい鎖パーツ");

    virtual ~LargeChainParts();
    virtual void kill();

    void breakChainParts();
    void initChainParts(TVec3f *, TVec3f *, TVec3f *, bool);
};