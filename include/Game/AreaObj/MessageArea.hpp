#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class MessageArea : public AreaObj {
public:
    MessageArea(int, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);

    /* 0x3C */ s32 mZoneID;
};
