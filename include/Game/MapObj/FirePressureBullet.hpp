#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class FirePressureBullet : public LiveActor {
public:
    FirePressureBullet(const char*);

    /* 0x08 */ virtual ~FirePressureBullet();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);

    void shotFireBullet(LiveActor*, const TPos3f&, const f32&, bool, bool);
    void exeFly();
    bool isCrash() const;

    TVec3f _8C;
    LiveActor* mFirePressure;  // 0x98
    f32 _9C;
    bool _A0;
    bool _A1;
};
