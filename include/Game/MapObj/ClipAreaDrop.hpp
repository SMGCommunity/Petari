#pragma once

#include "Game/MapObj/ClipArea.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "revolution/types.h"

class ClipAreaDrop : ClipArea {
public:
    ClipAreaDrop(const char*);
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();
    virtual ~ClipAreaDrop();

    void setBaseSize(f32);
    void exeWait();

    u32 _C0;
    f32 _C4;
};
