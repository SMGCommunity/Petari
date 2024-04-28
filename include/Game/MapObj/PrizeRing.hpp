#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"


namespace NrvPrizeRing {
    NERVE(PrizeRingStart);
    NERVE(PrizeRingLoop);
    NERVE(PrizeRingTimeout);
    NERVE(PrizeRingReadyToPass);
    NERVE(PrizeRingPass);
    NERVE(PrizeRingReadyToKill);    
};

class PrizeRing : public ModelObj {
    public:
        PrizeRing();

        virtual ~PrizeRing();
        virtual void init(const JMapInfoIter &);
        virtual void appear();
        virtual void kill();
        virtual void setLife(int);
        virtual void setNumber(int);
        virtual void setNervePass();
        virtual bool isReadyToPass() const;
        virtual bool isReadyToKill() const;
        virtual bool isPassed() const;
        virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
        virtual void playSound() const;
        virtual bool isOnTriggerTimeoutFlash() const;

        void exeStart();
        void exeLoop();
        void exeTimeout();
        inline void exeReadyToKill() {
            if (MR::isFirstStep(this)) {
                MR::hideModel(this);
            }
        }
        void exePass();

        int _90;

};
