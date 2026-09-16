#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class RailBlock : public LiveActor {
public:
    RailBlock(const char*);

    virtual ~RailBlock(){};
    virtual void init(const JMapInfoIter&);
    virtual void calcViewAndEntry();
    virtual void calcAndSetBaseMtx();

    void startMove();
    void stopMove();
    inline void exeWait();
    void exeMove();
    void exeTerminate();

    /* 0x8C */ s32 mRailSpeed;
    /* 0x90 */ s32 mMoveTime;
    /* 0x94 */ bool mIsConnectedToRail;
    /* 0x95 */ bool mIsBig;
};
