#pragma once

#include "Game/Boss/DinoPackunTailNode.hpp"

class DinoPackunTailPart : public DinoPackunTailNode {
public:
    DinoPackunTailPart(const char*, DinoPackun*);

    /* 0x08 */ virtual ~DinoPackunTailPart();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    /* 0x7C */ virtual void requestLockPosition();
    /* 0x80 */ virtual void requestUnLockPosition();
    /* 0x88 */ virtual JointController* createJointControllerOwn(LiveActor*, const char*);

    void exeWait();
    void exeLockPosition();

    /* 0xD4 */ f32 _D4;
    /* 0xD8 */ u8 _D8;
    /* 0xD9 */ u8 _D9;
    /* 0xDA */ u8 _DA;
    /* 0xDB */ u8 _DB;
};
