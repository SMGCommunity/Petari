#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class IceStep : public LiveActor {
public:
    IceStep(const char *);

    virtual ~IceStep();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);

    void exeWait();
    inline void exeHit() {
        s32 step = getNerveStep();

        switch (step) {
            case 5:
                MR::hideModel(this);
                break;
            case 0xF:
                kill();
                break;
        }
    }

    void setOn(u32, const TVec3f &, const TVec3f &);
    void destroy();
    void doHit();

    Mtx _8C;
    u16 mTimer;         // 0xBC
};

namespace NrvIceStep {
    NERVE_DECL(IceStepNrvHit, IceStep, IceStep::exeHit);
    NERVE_DECL(IceStepNrvWait, IceStep, IceStep::exeWait);
};