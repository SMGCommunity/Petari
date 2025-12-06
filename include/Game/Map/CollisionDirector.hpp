#pragma once

#include "Game/NameObj/NameObj.hpp"

class CollisionCategorizedKeeper;
class CollisionCode;

class CollisionDirector : public NameObj {
public:
    CollisionDirector();
    virtual ~CollisionDirector();

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void movement();

    CollisionCategorizedKeeper** mKeepers;  // 0xC
    CollisionCode* mCode;                   // 0x10
};

namespace MR {
    CollisionDirector* getCollisionDirector();
};
