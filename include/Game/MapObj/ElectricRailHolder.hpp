#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/Array.hpp"

enum ElectricRailType { ElectricRail_Red, ElectricRail_Yellow, ElectricRail_YellowMoving, ElectricRail_RedMoving };

class ElectricRailMoving;
class J3DMatPacket;
class ModelObj;

struct RailItem {
    /* 0x0 */ ElectricRailType mRailType;
    /* 0x4 */ const ElectricRailMoving* mHost;
    /* 0x8 */ J3DMatPacket* mMatPacket;
};

class ElectricRailHolder : public LiveActor {
public:
    /// @brief Creates a new `ElectricRailHolder`.
    /// @param pName A pointer to the null-terminated name of the object.
    ElectricRailHolder(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void calcAnim();
    virtual void draw() const;
    virtual void control();

    bool isTouchRail(const HitSensor*, TVec3f*, TVec3f*) const;
    void initDraw() const;
    void registerRail(const LiveActor*, ElectricRailType);
    ModelObj* createModel(ElectricRailType) const;

    /* 0x08C */ MR::FixedArray< ModelObj*, 4 > mRailModels;
    /* 0x09C */ MR::Vector< MR::FixedArray< RailItem*, 64 > > mRailItems;
    /* 0x1A0 */ bool mIsActive;
};

namespace ElectricRailFunction {
    void registerRail(const LiveActor*, ElectricRailType);
    bool isTouchRail(const HitSensor*, TVec3f*, TVec3f*);
    f32 getHitSensorRadius();
    u32 calcDisplayListSize(u32, s32);
};  // namespace ElectricRailFunction
