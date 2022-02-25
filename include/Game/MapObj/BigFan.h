#pragma once

#include "Game/LiveActor/LiveActor.h"

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

    ModelObj* mWindModel;   // _8C
    TVec3f _90;
    f32 mWindLength;        // _9C
    f32 _A0;
    bool mIsTeresaGalaxy;   // _A4
};

namespace NrvBigFan {
    NERVE(BigFanNrvStop);
    NERVE(BigFanNrvStart);
    NERVE(BigFanNrvWait);
};
