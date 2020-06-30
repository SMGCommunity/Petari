#pragma once

#include "Actor/NameObj/NameObj.h"

class ClippingJudge : public NameObj
{
public:
    ClippingJudge(const char *);

    virtual ~ClippingJudge();
    virtual void init(const JMapInfoIter &);
    virtual void movement();

    u8 _0[0x380];
};