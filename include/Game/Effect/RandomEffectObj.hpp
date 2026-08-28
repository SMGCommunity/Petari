#pragma once

#include "Game/Effect/SimpleEffectObj.hpp"

class RandomEffectObj : public SimpleEffectObj {
public:
    RandomEffectObj(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x48 */ virtual void control();

    virtual f32 getClippingRadius() const;
    virtual f32 getFarClipDistance() const;

    s32 getRandomTime() const;

private:
    /* 0x9C */ s32 mStep;
    /* 0xA0 */ s32 mSeStep;
    /* 0xA4 */ s32 mLifeTime;
    /* 0xA8 */ s32 mRandWidth;
};
