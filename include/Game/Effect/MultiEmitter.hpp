#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class EffectSystem;
class MultiEmitterCallBackBase;
class JPABaseEmitter;
class JPAParticleCallBack;
class ParticleEmitter;

class MultiEmitter {
public:
    MultiEmitter(const char*);
    MultiEmitter(const char*, MtxPtr, const TVec3f*, const TVec3f&);
    MultiEmitter(const char*, MtxPtr, const TVec3f&);
    MultiEmitter(const char*, const TVec3f*, const TVec3f*, const TVec3f*, const TVec3f&);

    void createEmitter();

    void forceDelete(EffectSystem*);
    void forceDeleteEmitter();
    void deleteEmitter();
    void create(EffectSystem*);

    void setHostMtx(MtxPtr);
    void setHostSRT(const TVec3f*, const TVec3f*, const TVec3f*);
    void setGlobalSRTMatrix(const MtxPtr, s32);

    void setBaseScale(f32);
    void setName(const char*);
    bool isEqualName(u16) const;
    void setLocalScale(const TVec3f&, s32);
    void setGlobalScale(const TVec3f&, s32);
    void setGlobalScale(f32, s32);
    void setGlobalRotationDegree(const TVec3f&, s32);
    void setRate(f32, s32);
    void setDrawOrder(s32);

    void setGlobalPrmColor(u8, u8, u8, s32);
    void setGlobalEnvColor(u8, u8, u8, s32);

    void setGlobalTranslation(const TVec3f&, s32);

    void createEmitterWithCallBack(MultiEmitterCallBackBase*);

    void setParticleCallBackPtr(JPAParticleCallBack*, s32);
    ParticleEmitter* getParticleEmitter(s32) const;
    void forceFollowOn();
    void forceFollowOff();
    void forceScaleOn();

    bool isValid() const;

    void onCreateSyncClipping();
    void onForceDeleteSyncClipping();

    /* 0x0 */ u32 _0;
    /* 0x4 */ u8 _4[0x38-0x4];
};
