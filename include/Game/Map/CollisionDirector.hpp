#pragma once

#include "Game/NameObj/NameObj.hpp"

class CollisionCategorizedKeeper;
class CollisionCode;

class CollisionDirector : public NameObj {
public:
    /// @brief Creates a new `CollisionDirector`.
    CollisionDirector();

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x14 */ virtual void movement();

    CollisionCategorizedKeeper* getCategoryKeeper(s32 index) {
        return mCategoryKeeper[index];
    }

    /* 0x0C */ CollisionCategorizedKeeper** mCategoryKeeper;
    /* 0x10 */ CollisionCode* mCode;
};

namespace MR {
    CollisionDirector* getCollisionDirector();
};  // namespace MR
