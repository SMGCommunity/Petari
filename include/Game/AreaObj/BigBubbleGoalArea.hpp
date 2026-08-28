#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class BigBubble;

class BigBubbleGoalArea : public AreaObj {
public:
    BigBubbleGoalArea(int, const char*);

    /* 0x08 */ virtual ~BigBubbleGoalArea() {};
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();
    /* 0x18 */ virtual void draw() const;

    /* 0x28 */ virtual const char* getManagerName() const {
        return "BigBubbleGoalArea";
    };

    void addBubble(BigBubble*);

    /* 0x3C */ TVec3f mTrans;
    /* 0x48 */ f32 mMeterCapacity;
    /* 0x4C */ f32 mMeterDisplayAmount;
    /* 0x50 */ f32 mMeterAmount;
};

namespace MR {
    bool checkBigBubbleGoal(BigBubble*);
};  // namespace MR
