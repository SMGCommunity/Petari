#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ArrowSwitch : public LiveActor {
public:
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

    /* 0x8C */ f32 _8C;
    /* 0x90 */ s32 mRotYTargetIndex;
    /* 0x94 */ f32 _94;
    /* 0x98 */ s32 mObjArg0;
    /* 0x9C */ bool mObjArg1;
    /* 0x9D */ bool _9D;
    /* 0x9E */ bool _9E;
    /* 0x9F */ bool _9F;
};
