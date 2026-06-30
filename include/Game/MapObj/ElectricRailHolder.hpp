#pragma once

#include "Game/LiveActor/LiveActor.hpp"

enum ElectricRailType {

};

class ElectricRailHolder : public LiveActor {
public:
    /// @brief Creates a new `ElectricRailHolder`.
    /// @param pName A pointer to the null-terminated name of the object.
    ElectricRailHolder(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void draw() const;
    virtual void calcAnim();
    virtual void control();

    bool isTouchRail(const HitSensor*, TVec3f*, TVec3f*) const;
    void initDraw() const;
    void registerRail(const LiveActor*, ElectricRailType);
    void createModel(ElectricRailType) const;

    /* 0x08C */ LiveActor* _8C[4];
    /* 0x09C */ void* _9C[64];
    /* 0x19C */ s32 _19C;
    /* 0x1A0 */ bool _1A0;
};

namespace ElectricRailFunction {
    void registerRail(const LiveActor*, ElectricRailType);
    bool isTouchRail(const HitSensor*, TVec3f*, TVec3f*);
    f32 getHitSensorRadius();
    u32 calcDisplayListSize(u32, s32);
};  // namespace ElectricRailFunction
