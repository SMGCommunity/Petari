#pragma once

#include "Game/LiveActor/ModelObj.hpp"

class ParabolicPath;

class MoguStone : public ModelObj {
public:
    MoguStone(const char*, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    /* 0x78 */ virtual void doBehavior();
    /* 0x7C */ virtual void startBreakSound();
    /* 0x80 */ virtual void startThrowLevelSound();

    void exeTaken();
    void exeThrow();
    void exeFall();
    bool isTaken();
    void emit(bool, const TVec3f&, const TVec3f&, f32);

    TQuat4f _90;
    TQuat4f _A0;
    TVec3f _B0;
    f32 _BC;
    bool _C0;
};

class ThrowingIce : public MoguStone {
public:
    ThrowingIce(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x78 */ virtual void doBehavior();
    /* 0x7C */ virtual void startBreakSound();
    /* 0x80 */ virtual void startThrowLevelSound();

    void emitIce(const TVec3f&, const TVec3f&, f32, const TVec3f&);

    /* 0xC4 */ ParabolicPath* mPath;
};
