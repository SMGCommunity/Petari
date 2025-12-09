#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry.hpp"
#include <revolution.h>

const Mtx qMtx2_1 = {};
const Mtx qMtx2_2 = {};

class WaterRoad : public LiveActor {
public:
    WaterRoad(const char*);

    u32 _8C;
    u32 _90;
    u32 _94;
    u32 _98;
    u8 _9C;
};

class WaterRoadModelInfo {
public:
    WaterRoadModelInfo(WaterRoad*, bool);

    void initPoints(WaterRoad*);
    void initDisplayList();
    void sendGD() const;
    void drawGD() const;
    void drawDirect(const WaterRoad*) const;
    s32 calcDemoDrawPointNum(const WaterRoad*) const;
    void loadMaterialHigh(const WaterRoad*) const;
    void loadMaterialLow() const;

    bool mIsLow;  // 0x0
    u32 _4;
    s32 _8;
    u32 _C;
    TVec3f* _10;
    TVec3s* _14;
    f32* _18;
    u32 mDLLength;      // 0x1C
    const void* mList;  // 0x20
};
