#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class FlexibleSphere : public LiveActor {
public:
    FlexibleSphere(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();
    virtual void control();
    virtual void draw() const;
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    /* 0x8C */ u16 mShakeTimer;
    /* 0x8E */ bool mGrowDirection;  // true = grow, false = shrink
    /* 0x8F */ bool mVanishOnShrinkEnd;
    /* 0x90 */ bool _90;  // this goes "unused" functionally, despite being "disabled" by arg4
    /* 0x91 */ bool mEnabled;
    /* 0x94 */ f32 mMaxScale;
    /* 0x98 */ f32 mMinScale;
    /* 0x9C */ f32 mScaleRate;
    /* 0xA0 */ s32 mTimer;
};
