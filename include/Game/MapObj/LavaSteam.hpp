#pragma once
#include "Game/LiveActor/LiveActor.hpp"

class LavaSteam : public LiveActor {
public:
    LavaSteam(const char* pName);
    virtual ~LavaSteam();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual void startClipped();
    virtual void endClipped();
    void startSteam();
    void exeWait();
    void exeWaitForSwitchOn();
    void exeSteam();
    void exeSteamEnd();

    TVec3f _8C;
    TVec3f _98;
};
