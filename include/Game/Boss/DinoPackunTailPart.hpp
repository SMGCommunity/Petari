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

    inline void exeLockPosition();
    inline void exeWait();

    f32 _D4;
    u8 _D8;
    u8 _D9;
    u8 _DA;
    u8 _DB;
};
