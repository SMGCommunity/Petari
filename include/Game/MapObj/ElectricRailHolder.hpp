#pragma once
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/HitSensor.hpp"
enum ElectricRailType {

};

class ElectricRailHolder : public LiveActor {
public:
    ElectricRailHolder(const char*);

    void init();
    void registerRail(const LiveActor*, ElectricRailType);

    virtual void calcAnim();
    virtual void control();

    void initDraw() const;
    void draw() const;
    void createModel(ElectricRailType) const;

    bool isTouchRail(const HitSensor*, TVec3f*, TVec3f*) const;

    u8* _8C[0x4];
    u8 _9C[0x100];
    u32 _19C;
    bool drawRail;
};

namespace ElectricRailFunction {

    void registerRail(const LiveActor*, ElectricRailType);
    bool isTouchRail(const HitSensor*, TVec3f*, TVec3f*);

    f32 getHitSensorRadius();
    u32 calcDisplayListSize(u32, s32);
};  // namespace ElectricRailFunction
