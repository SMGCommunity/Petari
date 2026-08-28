#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PlantPoint;
class PlantRailInfo;
class JUTTexture;

class PlantStalk : public LiveActor {
public:
    PlantStalk();

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x18 */ virtual void draw() const;
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x78 */ virtual void drawGrowUp() const;

    bool updateGrowUp();
    void calcPosAndAxisY(TVec3f*, TVec3f*, f32) const;

    void calcNearestPos(TVec3f*, const TVec3f&) const;
    f32 calcPlayerCoord() const;

    /* 0x8C */ s32 mNumPlantPoints;
    /* 0x90 */ PlantPoint** mPlantPoints;
    /* 0x94 */ PlantRailInfo* mRailInfo;
    /* 0x98 */ f32 mStalkLength;
    /* 0x9C */ f32 mGrowthPercent;
    /* 0xA0 */ f32 mGrowthRate;
    /* 0xA4 */ s32 mGrowthTimer;
    /* 0xA8 */ s32 mGrownPlantPoints;
};

class PlantStalkDrawInit : public NameObj {
public:
    PlantStalkDrawInit(const char*);

    /* 0x24 */ virtual void initDraw() const;

    /* 0x0C */ JUTTexture* mTexture;
};
