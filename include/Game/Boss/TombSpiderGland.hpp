#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class TombSpiderGland : public PartsModel {
public:
    TombSpiderGland(LiveActor*, const char*, const TVec3f&, const TVec3f&, const char*);

    virtual void init(const JMapInfoIter& rIter);

    void startActive();
    void startDamage();
    bool isActive() const;
    bool isDamage() const;

    void exeWait();
    void exeActiveWait();
    void exeDamageStart();
    void exeDamageWait();
};
