#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class TripodBossStepStartArea : public AreaObj {
public:
    TripodBossStepStartArea(int, const char*);

    /* 0x08 */ virtual ~TripodBossStepStartArea();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();
    /* 0x28 */ virtual const char* getManagerName() const;

    s32 _3C;
};
