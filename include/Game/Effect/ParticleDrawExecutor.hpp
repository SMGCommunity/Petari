#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class EffectSystem;
class NameObjAdaptor;

class ParticleDrawExecutor {
public:
    ParticleDrawExecutor(const EffectSystem*, bool);

    void initDraw();
    void draw3D() const;
    void draw2D() const;
    void drawIndirect() const;
    void drawAfterIndirect() const;
    void drawFor2DModel() const;
    void drawForBloomEffect() const;
    void drawAfterImageEffect() const;
    void drawWithViewMtx3D(const TPos3f&) const;
    void drawWithViewMtxAfterIndirect(const TPos3f&) const;
    void drawWithViewMtxForBloomEffect(const TPos3f&) const;
    void drawWithViewMtxAfterImageEffect(const TPos3f&) const;
    void initDrawAdaptor();

    /* 0x00 */ const EffectSystem* mHost;
    /* 0x04 */ NameObjAdaptor* _4;
    /* 0x08 */ NameObjAdaptor* _8;
    /* 0x0C */ NameObjAdaptor* _C;
    /* 0x10 */ NameObjAdaptor* _10;
    /* 0x14 */ NameObjAdaptor* _14;
    /* 0x18 */ NameObjAdaptor* _18;
    /* 0x1C */ NameObjAdaptor* _1C;
    /* 0x20 */ bool _20;
    /* 0x21 */ bool _21;
};
