#pragma once

#include "Game/Boss/DinoPackunTailNode.hpp"

class DinoPackunTailPart : public DinoPackunTailNode {
public:
    DinoPackunTailPart(const char*, DinoPackun*);

    virtual ~DinoPackunTailPart();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    virtual void requestLockPosition();
    virtual void requestUnLockPosition();
    virtual JointController* createJointControllerOwn(LiveActor*, const char*);

    void exeWait();
    void exeLockPosition();

    /* 0xD4 */ f32 _D4;
    /* 0xD8 */ u8 _D8;
    /* 0xD9 */ u8 _D9;
    /* 0xDA */ u8 _DA;
    /* 0xDB */ u8 _DB;
};
