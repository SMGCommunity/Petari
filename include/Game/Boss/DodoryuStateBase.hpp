#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Dodoryu;
class HitSensor;

class DodoryuStateBase : public LiveActor {
public:
    DodoryuStateBase(Dodoryu* pDodoryu, const char* pName) : LiveActor(pName), mDodoryu(pDodoryu) {
    }
    
    virtual void start();
    virtual void end();
    virtual void catchAttackSensor(HitSensor*, HitSensor*);
    virtual void catchHipDrop();
    virtual bool catchPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual void catchStartClipped();
    virtual void catchEndClipped();
    virtual void catchSendPlayerFlying(HitSensor*, HitSensor*);

    bool calcVerticalizedDir(TVec3f*, const TVec3f&);
    void calcAttackDir(TVec3f*, const TVec3f&, const TVec3f&);

    /* 0x8C */ Dodoryu* mDodoryu;
};
