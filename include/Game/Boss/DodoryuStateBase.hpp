#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Dodoryu;

class DodoryuStateBase : public LiveActor {
public:
    DodoryuStateBase(Dodoryu* pHost, const char* pName) : LiveActor(pName), mHost(pHost) {
    }

    virtual void start() {
    }

    virtual void end() {
    }

    virtual void catchAttackSensor(HitSensor*, HitSensor*) {
    }

    virtual void catchHipDrop() {
    }

    virtual bool catchPlayerAttack(u32, HitSensor*, HitSensor*) {
        return false;
    }

    virtual void catchStartClipped() {
    }

    virtual void catchEndClipped() {
    }

    virtual void catchSendPlayerFlying(HitSensor*, HitSensor*) {
    }

    void calcAttackDir(TVec3f*, const TVec3f&, const TVec3f&);
    bool calcVerticalizedDir(TVec3f*, const TVec3f&);

    /* 0x8C */ Dodoryu* mHost;
};
