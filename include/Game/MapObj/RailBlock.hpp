#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class RailBlock : public LiveActor {
public:
    RailBlock(const char*);

    /* 0x08 */ virtual ~RailBlock();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x20 */ virtual void calcViewAndEntry();
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    void startMove();
    void stopMove();
    inline void exeWait();
    void exeMove();
    void exeTerminate();

    s32 _8C;
    s32 _90;
    u8 _94;
    u8 _95;
};
