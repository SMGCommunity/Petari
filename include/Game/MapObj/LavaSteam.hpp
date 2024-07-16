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
    void exeSteam();

    TVec3f _8C;
    TVec3f _98;
};

namespace NrvLavaSteam {
    NERVE(HostTypeSteamEnd);
    NERVE_DECL(HostTypeSteam, LavaSteam, LavaSteam::exeSteam);
    NERVE_DECL(HostTypeWait, LavaSteam, LavaSteam::exeWait);
    NERVE_DECL_NULL(HostTypeWaitForSwitchOn);
};