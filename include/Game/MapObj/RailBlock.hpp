#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class RailBlock : public LiveActor {
public:
    RailBlock(const char*);

    virtual ~RailBlock();
    virtual void init(const JMapInfoIter&);
    virtual void calcViewAndEntry();
    virtual void calcAndSetBaseMtx();

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
