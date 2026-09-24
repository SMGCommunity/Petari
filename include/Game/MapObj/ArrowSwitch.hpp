#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ArrowSwitch : public LiveActor {
public:
    enum SwitchType {
        SwitchType_QuarterPlus = 0,
        SwitchType_QuarterMinus = 1,
        SwitchType_HalfPlus = 2,
        SwitchType_HalfMinus = 3,
    };

    enum Direction {
        Direction_Up = 0,
        Direction_Right = 1,
        Direction_Down = 2,
        Direction_Left = 3,
    };

    /// @brief Creates a new `ArrowSwitch`.
    /// @param pName A pointer to the null-terminated name of the object.
    ArrowSwitch(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    bool requestPunch(HitSensor*, HitSensor*);
    void listenOnSwitch();
    void listenOffSwitch();
    void exeWait();
    void exeRotate();
    void exeLock();
    bool isPlusLimit() const;
    bool isMinusLimit() const;
    s32 getOneStep() const;

    /* 0x8C */ f32 mRotAngle;
    /* 0x90 */ s32 mRotYTargetIndex;
    /* 0x94 */ f32 mRotSpeed;
    /* 0x98 */ s32 mSwitchType;
    /* 0x9C */ bool mLockAfterRotate;
    /* 0x9D */ bool mIsRotPlus;
    /* 0x9E */ bool _9E;
    /* 0x9F */ bool mIsPunch;
};
