#pragma once

#include "NameObj/NameObj.h"

class CameraDirector : public NameObj
{
public:
    CameraDirector(const char *);

    virtual void init(const JMapInfoIter &);
    virtual void movement();

    static s32 sUpdateCounter;
};