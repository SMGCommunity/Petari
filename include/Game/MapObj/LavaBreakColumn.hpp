#pragma once

#include "Game/MapObj/AnmModelObj.hpp"

class LavaBreakColumn : public AnmModelSwitchMoveEndKill {
public:
    LavaBreakColumn(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void control();

    /* 0xD0 */ TVec3f mEffectPos;
    /* 0xDC */ bool mIsPillarFallen;
};
