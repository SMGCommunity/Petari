#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/J3DGraphBase/J3DMaterial.hpp>

class SimpleJ3DModelDrawer : public LiveActor {
public:
    SimpleJ3DModelDrawer(NameObj*, const char*, const char*, int);

    void initDraw() const;

    /* 0x8C */ J3DMaterial* mMaterial;
    /* 0x90 */ J3DShape* mShape;
    /* 0x94 */ J3DShapeDraw* mShapeDraw;
};
