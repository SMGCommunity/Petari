#pragma once

#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"

class BossStinkBugFollowValidater : public BaseMatrixFollowValidater {
public:
    BossStinkBugFollowValidater(BossStinkBug* pStinkBug) {
        mStinkBug = pStinkBug;
    }

    virtual bool isValid(s32 id) const {
        return mStinkBug->isValidFollowId(id);
    }

    BossStinkBug* mStinkBug;
};
