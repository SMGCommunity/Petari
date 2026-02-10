#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Effect/SimpleEffectObj.hpp"


class EffectObjGravityDust : public SimpleEffectObj {
public:
    EffectObjGravityDust(const char*);
    virtual ~EffectObjGravityDust();

private:
    u8 mPad[(0xA4) - sizeof(SimpleEffectObj)];
};
