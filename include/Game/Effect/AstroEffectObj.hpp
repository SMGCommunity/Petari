#pragma once

#include "Game/Effect/SimpleEffectObj.hpp"

class AstroEffectObj : public SimpleEffectObj {
public:
    AstroEffectObj(const char*);

    /* 0x08 */ virtual ~AstroEffectObj();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    /* 0x80 */ virtual void setStateMove();

    void selectNrvWait();

    s32 _9C;
};
