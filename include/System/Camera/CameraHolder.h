#ifndef CAMERAHOLDER_H
#define CAMERAHOLDER_H

#include "Actor/NameObj/NameObj.h"

class CameraHolder : public NameObj
{
public:
    CameraHolder(const char *);

    s32 getIndexOf(const char *) const;

    s32 getIndexOfDefault() const;
};

#endif // CAMERAHOLDER_H