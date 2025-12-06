#pragma once

#include "Game/Effect/SimpleEffectObj.hpp"

class AstroEffectObj : public SimpleEffectObj {
public:
    AstroEffectObj(const char*);

    virtual ~AstroEffectObj();
    virtual void init(const JMapInfoIter&);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual void setStateMove();

    void selectNrvWait();

    s32 _9C;
};
