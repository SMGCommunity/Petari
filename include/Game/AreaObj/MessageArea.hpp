#pragma once

#include "Game/AreaObj/AreaObj.h"

class MessageArea : public AreaObj {
public:
    MessageArea(int, const char *);
    ~MessageArea();

    virtual void init(const JMapInfoIter &);

    s32 mZoneID;    // _3C
};
