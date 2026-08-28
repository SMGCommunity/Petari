#pragma once

#include "Game/MapObj/ClipArea.hpp"
#include "Game/MapObj/ClipAreaShape.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "revolution/types.h"

class ClipAreaDrop : public ClipArea {
public:
    ClipAreaDrop(const char*);
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x48 */ virtual void control();
    /* 0x08 */ virtual ~ClipAreaDrop();

    void setBaseSize(f32);
    void exeWait();

    ClipAreaShapeSphere* _C0;
    f32 _C4;
};
