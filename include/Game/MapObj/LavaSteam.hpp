#pragma once
#include "Game/LiveActor/LiveActor.hpp"

class LavaSteam : public LiveActor {
public:
    LavaSteam(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void startSteam();
    void exeWait();
    void exeWaitForSwitchOn();
    void exeSteam();
    void exeSteamEnd();

    /* 0x8C */ TVec3f mUp;
    /* 0x98 */ TVec3f mEffectScale;
};
