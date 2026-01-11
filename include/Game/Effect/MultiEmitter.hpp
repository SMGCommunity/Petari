#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class EffectSystem;
class MultiEmitterCallBackBase;
class JPAParticleCallBack;
class ParticleEmitter;

class MultiEmitter {
public:
    void forceDelete(EffectSystem*);
    void create(EffectSystem*);

    void setHostMtx(MtxPtr);
    void setHostSRT(const TVec3f*, const TVec3f*, const TVec3f*);
    void setGlobalSRTMatrix(const MtxPtr, s32);

    void setBaseScale(f32);
    void setLocalScale(const TVec3f&, s32);
    void setGlobalScale(const TVec3f&, s32);
    void setGlobalScale(f32, s32);
    void setGlobalRotationDegree(const TVec3f&, s32);

    void setGlobalPrmColor(u8, u8, u8, s32);
    void setGlobalEnvColor(u8, u8, u8, s32);

    void setGlobalTranslation(const TVec3f&, s32);
    void setRate(f32, s32);
    void setDrawOrder(s32);

    void createEmitterWithCallBack(MultiEmitterCallBackBase*);

    void setParticleCallBackPtr(JPAParticleCallBack*, s32);

    ParticleEmitter* getParticleEmitter(int) const;
    void deleteEmitter();
    void forceFollowOn();
    void forceFollowOff();
    void forceScaleOn();

    bool isValid() const;

    void onCreateSyncClipping();
    void onForceDeleteSyncClipping();
};
