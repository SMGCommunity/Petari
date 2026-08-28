#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class PalmIsland : public LiveActor {
public:
    PalmIsland(const char*);

    /* 0x08 */ virtual ~PalmIsland();
    /* 0x0C */ virtual void init(const JMapInfoIter&);

    void exeFloat();
    void exeWait();

    s32 _8C;     // 0x8C
    TVec3f _90;  // 0x90
    s32 _9C;     // 0x9C
};
