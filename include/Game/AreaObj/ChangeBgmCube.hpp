#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class ChangeBgmCube : public AreaObj {
public:
    ChangeBgmCube(int, const char*);

    /* 0x08 */ virtual ~ChangeBgmCube();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();

    /* 0x3C */ u8 _3C;
};
