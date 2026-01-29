#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class TombSpider;

class TombSpiderDemo {
public:
    TombSpiderDemo(TombSpider*);

    bool updateGateOpen();
    bool updateCocoonBreak();
    bool updateBattle1stStart();
    bool updateBattle1stEnd();
    bool updateBattle2ndStart();
    bool updateDeath();
    bool updateJumpRotateToPlayer();
    bool isStartDemoGateOpen() const;
    bool updateBattle1stStartJumpToPlayer();

    /* 0x00 */ TombSpider* mParent;
    /* 0x04 */ TPos3f mMtx;
    /* 0x34 */ f32 mRotateVel;
};
