#pragma once

#include "Game/NameObj/NameObj.h"

class CollisionCategorizedKeeper;
class CollisionCode;

class CollisionDirector : public NameObj {
public:
    CollisionDirector();
    virtual ~CollisionDirector();

    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void movement();

    CollisionCategorizedKeeper **mKeepers;  // _C
    CollisionCode *mCode;                   // _10
};

namespace MR {
    CollisionDirector *getCollisionDirector();
}
