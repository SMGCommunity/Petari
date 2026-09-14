#pragma once

#include "Game/NPC/NPCActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class LodCtrl;
class TalkMessageCtrl;

class Rabbit : public NPCActor {
public:
    enum SeVersion {
        SeVersion_NORMAL = 0,
        SeVersion_WATER = 1,
    };

    Rabbit(const char*);

    virtual ~Rabbit();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void calcRailPos(TVec3f*);
    bool isNeedTurn(const TVec3f&);
    void updateJump();

    void exeAppear();
    void exeAppearLand();
    void exeWait();
    void exeGoal();
    void exeFinish();
    void exeForwardLand();
    void exePreJump();
    void exeMove();
    void exeBackwardLand();
    void exePreJumpBack();
    void exeNear();
    void exeReaction();
    void exeTalk();
    void exeJumpV();
    void exeJumpH();

    /* 0x8C */ LodCtrl* mLodCtrl;
    /* 0x90 */ TalkMessageCtrl* mTalkCtrl;
};
