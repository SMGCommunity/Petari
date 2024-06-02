#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Enemy/RingBeam.hpp"


class BallBeamer : public LiveActor {
    public:
        BallBeamer(const char *);
        
        virtual void init(const JMapInfoIter &);
        virtual void syncSwitchOnA();
        virtual void syncSwitchOffA();
        virtual void syncSwitchOnB();
        virtual void setupAttack();
        virtual void tryAttack();
        virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
        virtual bool receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver);
        virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

        static void makeArchiveList(NameObjArchiveListCollector *, const JMapInfoIter &);
        void exeWait();
        void exeAttack();
        void exeInter();

        RingBeam** mBeams;
        bool _90;
        TMtx34f _98;


};

namespace NrvBallBeamer {
    NERVE(BallBeamerNrvWait);
    NERVE(BallBeamerNrvAttack);
    NERVE(BallBeamerNrvInter);
};