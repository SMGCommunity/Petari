#pragma once

#include "JSystem/JGeometry/TBox.hpp"
#include <JSystem/J2DGraph/J2DGrafContext.hpp>

class J2DOrthoGraph : public J2DGrafContext {
public:
    J2DOrthoGraph();
    J2DOrthoGraph(f32, f32, f32, f32, f32, f32);

    virtual ~J2DOrthoGraph() {
    }
    virtual void setPort();
    virtual J2DGrafType getGrafType() const;
    virtual void setLookat();

    void setOrtho(TBox2f const& bounds, f32 far, f32 near);

    TBox2f mOrtho;  // 0xBC
    f32 mNear;      // 0xCC
    f32 mFar;       // 0xD0
};

void J2DFillBox(f32 x, f32 y, f32 width, f32 height, JUtility::TColor color);
void J2DFillBox(JGeometry::TBox2< f32 > const& box, JUtility::TColor color);
