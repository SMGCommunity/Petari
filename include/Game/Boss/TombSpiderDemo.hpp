#pragma once

#include "Game/Boss/TombSpider.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>


class ActorCameraInfo;

class TombSpiderDemo {
public:
    TombSpiderDemo(TombSpider*);

    bool updateGateOpen();
    bool updateCocoonBreak();
    bool updateBattle1stStart();
    bool updateBattle1stEnd();
    bool updateBattle2ndStart();
    bool updateDeath();
    void updateJumpRotateToPlayer();
    bool isStartDemoGateOpen() const;
    bool updateBattle1stStartJumpToPlayer();

    inline ActorCameraInfo* getCameraInfo() { return mParent->mCameraInfo; }

    /* 0x00 */ TombSpider* mParent;
    /* 0x04 */ TPos3f mMtx;
    /* 0x34 */ f32 mRotateSpeed;
};
