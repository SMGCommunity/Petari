#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ArrowSwitchTarget;
class JMapIdInfo;

class ArrowSwitchMulti : public LiveActor {
public:
    /// @brief Creates a new `ArrowSwitchTarget`.
    /// @param pName A pointer to the null-terminated name of the object.
    ArrowSwitchMulti(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void registerTarget(ArrowSwitchTarget*);
    bool requestPunch(HitSensor*, HitSensor*);
    void exeWait();
    void exeRotate();

    /* 0x8C */ JMapIdInfo* mIdInfo;
    /* 0x90 */ ArrowSwitchTarget* mTargetArray[4];
    /* 0xA0 */ f32 _A0;
    /* 0xA4 */ f32 _A4;
    /* 0xA8 */ u32 _A8;
    /* 0xAC */ s32 _AC;
    /* 0xB0 */ bool _B0;
};

class ArrowSwitchTarget : public NameObj {
public:
    /// @brief Creates a new `ArrowSwitchTarget`.
    /// @param pName A pointer to the null-terminated name of the object.
    ArrowSwitchTarget(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();

    void onTarget();
    void offTarget();

    /* 0x0C */ JMapIdInfo* mIdInfo;
    /* 0x10 */ StageSwitchCtrl* mStageSwitchCtrl;
    /* 0x14 */ s32 mTargetIndex;
};
