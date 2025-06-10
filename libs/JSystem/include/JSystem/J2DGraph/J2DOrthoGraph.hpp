#pragma once

#include <JSystem/J2DGraph/J2DGrafContext.hpp>

class J2DOrthoGraph : public J2DGrafContext {
public:
    J2DOrthoGraph();
    J2DOrthoGraph(f32, f32, f32, f32, f32, f32);

    void setPort();

    TBox2f _BC;
    f32 mNear;          // 0xCC
    f32 mFar;           // 0xD0
};
