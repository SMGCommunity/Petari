#pragma once

#include "Game/LiveActor/LiveActor.hpp"

namespace NrvBigFan {
    NERVE(BigFanNrvStop);
    NERVE(BigFanNrvStart);
    NERVE(BigFanNrvWait);
};


class BigFan : public LiveActor {
public:
    BigFan(const char *);

    virtual ~BigFan();
    virtual void init(const JMapInfoIter &);
    virtual void control();

    void initWindModel();
    void calcWindInfo(TVec3f *, const TVec3f &);
    void start();
    void exeStart();
    void exeWait();
    
    inline bool isStartOrWait() {
        bool flag = false; 
        if (isNerve(&NrvBigFan::BigFanNrvStart::sInstance) || isNerve(&NrvBigFan::BigFanNrvWait::sInstance)) {
            flag = true;
        }

        return flag;
    }

    ModelObj* mWindModel;   // 0x8C
    TVec3f _90;
    f32 mWindLength;        // 0x9C
    f32 _A0;
    bool mIsTeresaGalaxy;   // 0xA4
};
