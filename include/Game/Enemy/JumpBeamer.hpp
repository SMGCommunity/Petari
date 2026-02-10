#pragma once

#include "Game/Enemy/JumpGuarder.hpp"

class RingBeam;

class JumpBeamer : public JumpEmitter {
public:
    JumpBeamer(const char*);

    virtual ~JumpBeamer();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
