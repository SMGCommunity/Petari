#pragma once

#include "Actor/NameObj/NameObj.h"

class CameraHolder : public NameObj
{
public:
    CameraHolder(const char *);

    s32 getIndexOf(const char *) const;

    s32 getIndexOfDefault() const;
};