#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/J3DGraphBase/J3DMaterial.hpp>

class SimpleJ3DModelDrawer : public LiveActor {
public:
    SimpleJ3DModelDrawer(NameObj*, const char*, const char*, int);

    virtual ~SimpleJ3DModelDrawer();

    void initDraw() const;

    J3DMaterial* mMaterial;    // 0x8C
    J3DShape* mShape;          // 0x90
    J3DShapeDraw* mShapeDraw;  // 0x94
};
