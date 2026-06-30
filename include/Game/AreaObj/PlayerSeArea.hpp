#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class PlayerSeArea : public AreaObj {
public:
    PlayerSeArea(int, const char*);

    virtual ~PlayerSeArea();
    virtual void init(const JMapInfoIter&);
    virtual void movement();

    virtual const char* getManagerName() const {
        return "PlayerSeArea";
    }

    /* 0x3C */ s32 _3C;
};
