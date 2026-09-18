#pragma once

class EffectSystem;
class NameObjAdaptor;

class ParticleCalcExecutor {
public:
    ParticleCalcExecutor(const EffectSystem* pEffectSystem, bool createAdaptors);
    void movementNormal();
    void movementIgnorePause3D();
    void movementIgnorePause2D();
    void movementCheckUpdate();
    void requestMovementOnPauseIgnore();
    void initMovementAdaptor();

    /* 0x00 */ const EffectSystem* mEffectSystem;
    /* 0x04 */ NameObjAdaptor* mNormalAdaptor;
    /* 0x08 */ NameObjAdaptor* mIgnorePause3DAdaptor;
    /* 0x0C */ NameObjAdaptor* mIgnorePause2DAdaptor;
    /* 0x10 */ NameObjAdaptor* mCheckUpdateAdaptor;
    /* 0x14 */ bool mMovementEnabled;
    /* 0x15 */ bool mUpdateRequested;
};
