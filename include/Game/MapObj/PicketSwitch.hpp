#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PicketSwitch : public LiveActor {
    public:
        PicketSwitch(const char *);

        virtual ~PicketSwitch();
        virtual void init(const JMapInfoIter &);
        virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

        inline void exeWait() {
            if (MR::isFirstStep(this)) {
                MR::startBrk(this, "Second");
                MR::setBrkFrameAndStop(this, 0.0f);
            }
        }
        void exeFirstDrop();
        void exeLastDrop();
};

namespace NrvPicketSwitch {
    NERVE(PicketSwitchNrvWait);
    NERVE(PicketSwitchNrvFirstDrop);
    NERVE(PicketSwitchNrvLastDrop);
};