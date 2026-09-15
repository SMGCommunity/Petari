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
    /* 0x15C */ s32 _15C;
    /* 0x15C */ bool _160;
    /* 0x15C */ bool _162;
    /* 0x15C */ s32 _164;
    /* 0x15C */ f32 _168;
    /* 0x15C */ f32 _16C;
    /* 0x15C */ f32 _170;
    /* 0x15C */ TVec3f _174;
    /* 0x15C */ f32 _180;
    /* 0x15C */ TQuat4f _A0;
    /* 0x15C */ bool _E4;
    /* 0x15C */ bool _D9;
    /* 0x15C */ bool _DB;
};
