#pragma once

#include "Game/Enemy/JumpGuarder.hpp"

class RingBeam;

class JumpBeamer : public JumpEmitter {
public:
    JumpBeamer(const char*);

    /* 0x08 */ virtual ~JumpBeamer();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void syncSwitchOffB();
    void exeHide();
    void exeUp();
    void exeWait();
    void exeDown();
    void exeHopStart();
    void exeHopWait();
    void exeHopJump();
    void exeHopEnd();
    void exePreOpen();
    void exeOpen();
    void exeClose();
    void exeInter();

    RingBeam** mBeams;  // 0xDC
};
