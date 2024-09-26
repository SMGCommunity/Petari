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
        virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);

        void setLife(int);
        void setNumber(int);
        void setNervePass();
        bool isReadyToPass() const;
        bool isReadyToKill() const;
        void exeStart();
        void exeLoop();
        void exeTimeout();
        inline void exeReadyToKill() {
            if (MR::isFirstStep(this)) {
                MR::hideModel(this);
            }
        }
        void exePass();
        bool isPassed() const;
        void playSound() const;            
        bool isOnTriggerTimeoutFlash() const;

        int _90;

};
