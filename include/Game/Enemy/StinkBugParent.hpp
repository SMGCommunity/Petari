#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Enemy/StinkBugBase.hpp"


class StinkBugParent : public StinkBugBase {
public:
    StinkBugParent(const char*);
    virtual ~StinkBugParent();

private:
    u8 mPad[(0xC4) - sizeof(StinkBugBase)];
};
