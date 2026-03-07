#pragma once

#include "Game/LiveActor/LiveActor.hpp"

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

    TVec3f _8C;
    LiveActor* mFirePressure;  // 0x98
    f32 _9C;
    bool _A0;
    bool _A1;
};
