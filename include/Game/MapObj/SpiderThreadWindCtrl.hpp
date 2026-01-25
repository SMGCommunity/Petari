#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class SpiderThreadWindCtrl {
public:
    SpiderThreadWindCtrl();

    void update();
    void startWindBattle();
    void startWindChance();
    void startWindBattleEnd();

    s32 getTimeToStartWind() const;
    s32 getWindTime() const;

    /* 0x00 */ f32 mWindTargetSpeed;
    /* 0x04 */ f32 mWindSpeed;
    /* 0x08 */ f32 mWindAccel;
    /* 0x0C */ TVec3f mWindDirection;
    /* 0x18 */ TVec3f mWind;
};
