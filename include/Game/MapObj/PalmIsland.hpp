#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class PalmIsland : public LiveActor {
public:
    PalmIsland(const char*);

    virtual ~PalmIsland();
    virtual void init(const JMapInfoIter&);

    void exeFloat();
    void exeWait();

    /* 0x8C */ s32 _8C;
    /* 0x90 */ TVec3f _90;
    /* 0x9C */ s32 _9C;
};
