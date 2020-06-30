#pragma once

#include "Actor/Clipping/ClippingActorInfo.h"
#include "Actor/NameObj/NameObj.h"

class ClippingGroupHolder : public NameObj
{
public:
    ClippingGroupHolder();

    virtual ~ClippingGroupHolder();
    virtual void movement();

    void createAndAdd(ClippingActorInfo *, const JMapInfoIter &, s32);

    u8 _0[0x8];
};