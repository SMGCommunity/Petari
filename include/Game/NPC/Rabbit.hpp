#pragma once

#include "Game/NPC/NPCActor.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class LodCtrl;
class TalkMessageCtrl;

class Rabbit : public NPCActor {
public:
    enum SeVersion {
        SeVersion_NORMAL = 0,
        SeVersion_WATER = 1,
    };

    enum Behavior {
        Behavior_Wait = 0,
        Behavior_Unknown = 1,
        Behavior_Demo1 = 2,
        Behavior_Demo2 = 3,
        Behavior_UpJump = 4,
        Behavior_LongJump = 5,
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

    /* 0x15C */ s32 mBehavior;
    /* 0x160 */ bool _160;
    /* 0x162 */ u16 _162;
    /* 0x164 */ u16 _164;
    /* 0x168 */ f32 _168;
    /* 0x16C */ f32 _16C;
    /* 0x170 */ f32 _170;
    /* 0x174 */ TVec3f _174;
    /* 0x180 */ f32 _180;
};
