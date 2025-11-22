#pragma once

#include "Game/MapObj/ClipArea.hpp"
#include "Game/MapObj/ClipAreaShape.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "revolution/types.h"

class ClipAreaDrop : public ClipArea {
public:
    ClipAreaDrop(const char*);
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();
    virtual ~ClipAreaDrop();

    void setBaseSize(f32);
    void exeWait();

    ClipAreaShapeSphere* _C0;
    f32 _C4;
};
