#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class MessageArea : public AreaObj {
public:
    MessageArea(int, const char*);
    ~MessageArea();

    virtual void init(const JMapInfoIter&);

    s32 mZoneID;  // 0x3C
};
