#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Enemy/StinkBugBase.hpp"


class StinkBugSmall : public StinkBugBase {
public:
    StinkBugSmall(const char*);
    virtual ~StinkBugSmall();

private:
    u8 mPad[(0xC8) - sizeof(StinkBugBase)];
};
