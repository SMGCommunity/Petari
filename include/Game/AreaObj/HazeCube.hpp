#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class HazeCube : public AreaObj {
public:
    HazeCube(int, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);

    /* 0x28 */ virtual const char* getManagerName() const {
        return "HazeCube";
    }

    /* 0x3C */ f32 mDistance;
};
