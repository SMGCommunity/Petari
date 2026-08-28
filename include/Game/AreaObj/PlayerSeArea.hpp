#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class PlayerSeArea : public AreaObj {
public:
    PlayerSeArea(int, const char*);

    /* 0x08 */ virtual ~PlayerSeArea();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();

    /* 0x28 */ virtual const char* getManagerName() const {
        return "PlayerSeArea";
    }

    /* 0x3C */ s32 _3C;
};
