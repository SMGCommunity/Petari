#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class RockCreator;

class Rock : public LiveActor {
public:
    Rock(f32, const char*);

    enum Type { NormalRock = 0, WanwanRolling = 1, WanwanRollingMini = 2, WanwanRollingGold = 3 };

    virtual ~Rock();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    static s32 getAppearFrame();
    static Type getType(const JMapInfoIter&);
    static s32 getAppearStarPieceNum(Type) NO_INLINE;
    void initMapToolInfo(const JMapInfoIter&);
    void initModel();
    void initSensor();
    void initEffect();
    bool isBreakByWall() const;
    bool move(f32);
    void calcBaseMtx(TPos3f*) const;
    bool isInClippingRange() const;
    void startSoundWanwanVoice();
    void startRollLevelSound(bool);
    bool tryFreeze(const Nerve*);
    void setBtkForEnvironmentMap(LiveActor*, const char*);
    void rumblePadAndCamera();
    void setNerveBreak(bool);
    void updateRotateX(f32);
    void appearStarPiece();
    void moveOnRail(f32, f32, bool);
    bool isForceInvalidBindSection() const;
    bool tryBreakReachedGoal();
    void exeAppear();
    void exeAppearMoveInvalidBind();
    void exeMove();
    void exeMoveInvalidBind();
    void exeBreak();
    void exeFreeze();

    RockCreator* mParentCreator;  // 0x8C
    Type mRockType;               // 0x90
    ModelObj* mBreakModel;        // 0x94
    f32 _98;
    u8 _9C;
    bool _9D;
    TVec3f _A0;
    f32 _AC;
    f32 _B0;
    TMtx34f _B4;
    TVec3f _E4;
    f32 _F0;
    s32 mCurrentRailPoint;  // 0xF4
    TVec3f _F8;
    TVec3f _104;
    u32 _110;
    u8 _114;
    u8 _115;
    u8 _116;
    u8 _117;
    s32 mBarkTimer;  // 0x118
    u32 _11C;
    TVec3f _120;
    const Nerve* _12C;
    u32 _130;
    u32 _134;
    f32 _138;
    s32 mRollSoundTimer;  // 0x13C
    s32 _140;
};
