#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class ElectricPressureBullet : public LiveActor {
public:
    ElectricPressureBullet(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void shotElectricBullet(LiveActor*, const TPos3f&, const f32&);
    void exeFly();

    TVec3f _8C;
    LiveActor* _98;
    f32 _9C;
};
