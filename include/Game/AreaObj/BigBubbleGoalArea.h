#pragma once

#include "Game/AreaObj/AreaObj.h"

class BigBubble;

class BigBubbleGoalArea : public AreaObj {
    
    BigBubbleGoalArea(int, const char *);
    virtual ~BigBubbleGoalArea();

    virtual void init(const JMapInfoIter &);
    virtual void movement();
    virtual void draw() const;
    virtual const char *getManagerName() const;

    void addBubble(BigBubble *);

    TVec3f mTranslation;    // _3C
    f32 _48;
    f32 _4C;
    f32 _50;
};
