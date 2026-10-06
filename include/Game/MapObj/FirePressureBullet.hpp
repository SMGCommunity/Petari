#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class FirePressureBullet : public LiveActor {
public:
    FirePressureBullet(const char*);

    virtual ~FirePressureBullet();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void shotFireBullet(LiveActor*, const TPos3f&, const f32&, bool, bool);
    void exeFly();
    bool isCrash() const;

    /* 0x8C */ TVec3f _8C;
    /* 0x98 */ LiveActor* mFirePressure;
    /* 0x9C */ f32 _9C;
    /* 0xA0 */ bool _A0;
    /* 0xA1 */ bool _A1;
};
