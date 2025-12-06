#pragma once

#include "Game/LiveActor/ActorAnimKeeper.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

class OctahedronBezierSurface;

class BigBubble : public LiveActor {
public:
    BigBubble(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initSensor();
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAnim();
    virtual void draw() const;
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual MtxPtr getBaseMtx() const;
    virtual ~BigBubble();

    void setActionType(s32);
    void exeMerged();
    void exeGoal();
    void exeEscape();
    void exeBreak();
    void exeCapture();
    void endCapture();
    void exeWait();
    void exeAppear();
    void updateNormalVelocity();
    bool requestCancelBind();
    bool tryBreakEnd();
    f32 getBaseRadius() const;
    bool tryGoal();
    bool isMerged();
    bool isPushable() const;
    bool tryAppearEnd();
    bool tryAutoBreak();
    bool tryMergeEnd();
    bool tryMergedCancel();
    bool isDraw() const;
    bool isBindMario() const;
    bool tryBreak();
    bool tryEscape();
    bool tryEscapeEnd();
    bool isEnemyAttackBreakable() const;
    void setHost(LiveActor*);
    void addCoriolisAccel();
    void calcLocalDirection(TVec3f*, long);
    void setScale(f32);
    void resetDeformVelocity();
    // ...

    OctahedronBezierSurface* _8C;
    u32 _90;
    LiveActor* _94;
    u32 _98;
    LiveActor* _9C;  // Hmm...
    u8 _A0[24];
    TMtx34f _B8;
    u8 _E8[48];  // TMtx34f or TPos3f ?
    TPos3f _118;
    TQuat4f _148;
    TQuat4f _158;
    TVec3f _168;
    TVec3f _174;
    TVec3f _180;
    u8 _18C[24];
    TVec3f _1A4;
    TVec3f _1B0;
    u8 _1BC[60];
    f32 _1F8;
    f32 _1FC;
    u8 _200[40];
    bool _228;
    bool _229;
    bool _22A;
    bool _22B;
    bool _22C;
    bool _22D;
    bool _22E;
    bool _22F;
    bool _230;
    bool _231;
    bool _232;
};

class OctahedronBezierSurface {
public:
    OctahedronBezierSurface(u32);
};
