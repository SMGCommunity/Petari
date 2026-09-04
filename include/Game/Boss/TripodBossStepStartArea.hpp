#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class TripodBossStepStartArea : public AreaObj {
public:
    TripodBossStepStartArea(int, const char*);

    virtual ~TripodBossStepStartArea();
    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual const char* getManagerName() const;

    /* 0x3C */ s32 _3C;
};
