#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Dodoryu;

class DodoryuStateBase : public LiveActor {
public:
    DodoryuStateBase(Dodoryu* pHost, const char* pName) : LiveActor(pName), mHost(pHost) {
    }

    /* 0x78 */ virtual void start() {
    }

    /* 0x7C */ virtual void end() {
    }

    /* 0x80 */ virtual void catchAttackSensor(HitSensor*, HitSensor*) {
    }

    /* 0x84 */ virtual void catchHipDrop() {
    }

    /* 0x88 */ virtual bool catchPlayerAttack(u32, HitSensor*, HitSensor*) {
        return false;
    }

    /* 0x8C */ virtual void catchStartClipped() {
    }

    /* 0x90 */ virtual void catchEndClipped() {
    }

    /* 0x94 */ virtual void catchSendPlayerFlying(HitSensor*, HitSensor*) {
    }

    void calcAttackDir(TVec3f*, const TVec3f&, const TVec3f&);
    bool calcVerticalizedDir(TVec3f*, const TVec3f&);

    /* 0x8C */ Dodoryu* mHost;
};
