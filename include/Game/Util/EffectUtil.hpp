#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution.h>

class HitSensor;
class LayoutActor;
class LiveActor;
class MultiEmitter;
class MultiEmitterCallBackBase;
class MultiEmitterParticleCallBack;
class Triangle;

namespace MR {
    void requestEffectStopSceneStart();
    void requestEffectStopSceneEnd();
    void addEffect(LiveActor*, const char*);
    MultiEmitter* getEffect(const LiveActor*, const char*);
    MultiEmitter* getEffect(const LayoutActor*, const char*);
    bool isExistEffectKeeper(const LiveActor*);
    bool isExistEffectKeeper(const LayoutActor*);
    MultiEmitter* emitEffect(LiveActor*, const char*);
    MultiEmitter* emitEffectWithScale(LiveActor*, const char*, f32, s32);
    MultiEmitter* emitEffectWithEmitterCallBack(LiveActor*, const char*, MultiEmitterCallBackBase*);
    MultiEmitter* emitEffectWithParticleCallBack(LiveActor*, const char*, MultiEmitterParticleCallBack*);
    MultiEmitter* tryEmitEffect(LiveActor*, const char*);
    bool tryDeleteEffect(LiveActor*, const char*);
    void deleteEffect(LiveActor*, const char*);
    void forceDeleteEffect(LiveActor*, const char*);
    void deleteEffectAll(LiveActor*);
    void forceDeleteEffectAll(LiveActor*);
    bool isRegisteredEffect(const LiveActor*, const char*);
    bool isEffectValid(const LiveActor*, const char*);
    void onDrawEffect(LiveActor*);
    void offDrawEffect(LiveActor*);
    void pauseOffEffectAll(LiveActor*);
    void onEmitEffectSyncClipping(LiveActor*, const char*);
    void onForceDeleteEffectSyncClipping(LiveActor*, const char*);
    void setEffectName(LiveActor*, const char*, const char*);
    void setEffectHostSRT(LiveActor*, const char*, const TVec3f*, const TVec3f*, const TVec3f*);
    void setEffectHostMtx(LiveActor*, const char*, MtxPtr);
    void setEffectBaseScale(LiveActor*, const char*, f32);
    void setEffectLocalScale(LiveActor*, const char*, const TVec3f&);
    void setEffectColor(LiveActor*, const char*, u8, u8, u8, u8, u8, u8);
    void setEffectPrmColor(LiveActor*, const char*, u8, u8, u8);
    void setEffectEnvColor(LiveActor*, const char*, u8, u8, u8);
    void emitEffectHit(LiveActor*, const TVec3f&, const char*);
    void emitEffectHit(LiveActor*, const TVec3f&, const TVec3f&, const char*);
    void emitEffectHit(LiveActor*, MtxPtr, const char*);
    void emitEffectHitBetweenSensors(LiveActor*, const HitSensor*, const HitSensor*, f32, const char*);
    void initEffectAfterPlacement(LiveActor*);
    void updateEffectFloorCode(LiveActor*, const Triangle*);
    void updateEffectFloorCodeLineToMap(LiveActor*, const TVec3f&, const TVec3f&);
    void updateEffectFloorCodeLineToMap(LiveActor*, f32, f32);
    void initEffectSyncBck(LiveActor*, const char*, const char* const*);
    void addEffectHitNormal(LiveActor*, const char*);
};  // namespace MR
