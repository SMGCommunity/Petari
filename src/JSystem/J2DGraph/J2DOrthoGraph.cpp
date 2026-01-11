#include "JSystem/J2DGraph/J2DOrthoGraph.hpp"

J2DOrthoGraph::J2DOrthoGraph() : J2DGrafContext(0, 0, 0, 0) {
    this->setLookat();
}

J2DOrthoGraph::J2DOrthoGraph(f32 x, f32 y, f32 width, f32 height, f32 far, f32 near) : J2DGrafContext(x, y, width, height) {
    mOrtho = JGeometry::TBox2< f32 >(0, 0, width, height);
    mNear = -near;
    mFar = -far;
    this->setLookat();
}

void J2DOrthoGraph::setPort() {
    this->J2DGrafContext::setPort();
    C_MTXOrtho(mMtx44, mOrtho.i.y, mOrtho.f.y, mOrtho.i.x, mOrtho.f.x, mNear, mFar);
    GXSetProjection(mMtx44, GX_ORTHOGRAPHIC);
}

void J2DOrthoGraph::setOrtho(JGeometry::TBox2< f32 > const& bounds, f32 far, f32 near) {
    mOrtho = bounds;
    mNear = -near;
    mFar = -far;
}

void J2DOrthoGraph::setLookat() {
    PSMTXIdentity(mPosMtx);
    GXLoadPosMtxImm(mPosMtx, 0);
}

void J2DFillBox(JGeometry::TBox2< f32 > const& box, JUtility::TColor color) {
    J2DOrthoGraph oGrph;
    oGrph.setColor(color);
    oGrph.fillBox(box);
}

void J2DFillBox(f32 x, f32 y, f32 width, f32 height, JUtility::TColor color) {
    J2DFillBox(JGeometry::TBox2< f32 >(x, y, x + width, y + height), color);
}
