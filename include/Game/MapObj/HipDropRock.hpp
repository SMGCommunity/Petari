#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"

class HipDropRock : public LiveActor {
    public:
        HipDropRock(const char *);

        virtual void init(const JMapInfoIter &);
        virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

        void initBreakModel();
        void initItem(const JMapInfoIter &);
        void appearItem();
        void exeBreak();
        inline void ValClip1() {
            if (MR::isFirstStep(this)) {
                MR::validateClipping(this);
            }
        }
        inline void ValClip2() {
            if (MR::isFirstStep(this)) {
                MR::validateClipping(this);
            }
        }

        ModelObj* mModel; //_8C
        TMtx34f _90;
        s32 _C0;
        s32 _C4;
        bool _C8;
};

namespace NrvHipDropRock {
    NERVE(HipDropRockNrvWait);
    NERVE(HipDropRockNrvBreak);
    NERVE(HipDropRockNrvWreck);
};
