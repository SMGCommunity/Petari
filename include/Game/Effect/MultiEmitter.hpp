#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class EffectSystem;
class MultiEmitterCallBackBase;
class JPAParticleCallBack;

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

    void setGlobalPrmColor(u8, u8, u8, s32);
    void setGlobalEnvColor(u8, u8, u8, s32);

    void setGlobalTranslation(const TVec3f&, s32);

    void createEmitterWithCallBack(MultiEmitterCallBackBase*);

    void setParticleCallBackPtr(JPAParticleCallBack*, s32);

    bool isValid() const;

    void onCreateSyncClipping();
    void onForceDeleteSyncClipping();
};
