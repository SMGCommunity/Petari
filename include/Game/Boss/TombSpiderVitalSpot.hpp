#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class TombSpiderVitalSpot : public PartsModel {
public:
    TombSpiderVitalSpot(LiveActor*, const char*, const TVec3f&, const TVec3f&, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);

    void startChance();
    void endChance();
    void startDamage();
    void recover();
    bool isDamage() const;

    void exeWait();
    void exeChanceWait();
    void exeDamageStart();
    void exeDamageWait();
    void exeRecover();
};
