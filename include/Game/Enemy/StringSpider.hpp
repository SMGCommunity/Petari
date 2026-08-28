#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpinHitController;
class AnimScaleController;
class AnimStampController;
class StringSpiderString;
class FixedPosition;

class StringSpider : public LiveActor {
public:
    StringSpider(const char*);

    /* 0x08 */ virtual ~StringSpider();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x1C */ virtual void calcAnim();
    /* 0x2C */ virtual void kill();
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    s32 getSearchToAttackTime();
    void doSwingAction(f32, bool);
    void reactDPD();
    void reactStringForce();
    bool isOverStringLength(TVec3f*, f32*);
    TVec3f calcToRootVector() NO_INLINE;
    void calcPosture(f32, bool);
    void tearDownDpdGripped();
    void extendString();
    void shortenString(f32);
    bool reactSpinHit(HitSensor*, HitSensor*);
    void startSwingLevelSound();

    void exeWait();
    void endWait();
    void exeDownStart();
    void exeDownEnd();
    void exeDown();
    void exeUp();
    void exeSearch();
    void exeAttack();
    void exeHit();
    void exeSpinned();
    void exeTrampleFall();
    void exeWallHitDeath();
    void exeBlow();
    void exeDpdGripped();
    void endDpdGripped();

    /* 0x8C */ SpinHitController* mSpinHitController;
    /* 0x90 */ AnimScaleController* mScaleController;
    /* 0x94 */ AnimStampController* mStampController;
    /* 0x98 */ StringSpiderString* mString;
    /* 0x9C */ FixedPosition* mFixedPos;
    /* 0xA0 */ TQuat4f _A0;
    /* 0xB0 */ f32 _B0;
    /* 0xB4 */ f32 _B4;
    /* 0xB8 */ bool _B8;
    /* 0xBC */ s32 _BC;
    /* 0xC0 */ TVec3f* _C0;
    /* 0xC4 */ TVec3f* _C4;
};

class StringSpiderString : public LiveActor {
public:
    StringSpiderString(const char*);

    /* 0x08 */ virtual ~StringSpiderString();
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    /* 0x8C */ TQuat4f _8C;
    /* 0x9C */ TVec3f _9C;
};
