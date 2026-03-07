#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class BigBubble;

class BigBubbleGoalArea : public AreaObj {
public:
    BigBubbleGoalArea(int, const char*);

    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual void draw() const;
    virtual const char* getManagerName() const { return "BigBubbleGoalArea"; };

    void addBubble(BigBubble*);

    /* 0x3C */ TVec3f mTranslation;
    /* 0x48 */ f32 mMeterCapacity;
    /* 0x4C */ f32 mMeterDisplayAmount;
    /* 0x50 */ f32 mMeterAmount;
};

namespace MR {
    bool checkBigBubbleGoal(BigBubble*);
}
