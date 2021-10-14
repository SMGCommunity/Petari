#pragma once

#include "Game/AreaObj/AreaObj.h"

class ChangeBgmCube : public AreaObj {
public:
    ChangeBgmCube(int, const char *);

    virtual ~ChangeBgmCube();
    virtual void init(const JMapInfoIter &);
    virtual void movement();

    u8 _3C;
};