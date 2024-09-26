#pragma once

#include <revolution.h>
#include <JSystem/JGeometry.hpp>

class ParticleEmitter;
class LiveActor;
class HitSensor;
class SingleEmitter;
class MultiEmitter;
class LayoutActor;
class MultiEmitterParticleCallBack;

namespace MR {
    namespace Effect {
        void deleteParticleEmitter(ParticleEmitter *);
        void setLinkSingleEmitter(ParticleEmitter *, SingleEmitter *);
    };

    bool isExistEffectKeeper(const LiveActor *);

    bool isRegisteredEffect(const LiveActor *, const char *);

    bool isEffectValid(const LiveActor *, const char *);
    void deleteEffect(LiveActor *, const char *);

    void pauseOffEffectAll(LiveActor *);

    void deleteEffect(LiveActor *, const char *);
    void deleteEffectAll(LiveActor *);

    void emitEffect(LiveActor *, const char *);

    MultiEmitter* getEffect(const LayoutActor *, const char *);

    void addEffect(LiveActor *, const char *);

    void onDrawEffect(LiveActor *);
    void offDrawEffect(LiveActor *);
    void forceDeleteEffect(LiveActor *, const char *);
    void forceDeleteEffectAll(LiveActor *);

    void addEffectHitNormal(LiveActor *, const char *);

    void setEffectHostMtx(LiveActor *, const char *, MtxPtr);

    void emitEffectHitBetweenSensors(LiveActor *, const HitSensor *, const HitSensor *, f32, const char *);

    void initEffectAfterPlacement(LiveActor *);

    void setEffectHostSRT(LiveActor *, const char *, const TVec3f *, const TVec3f *, const TVec3f *);

    void emitEffectWithScale(LiveActor *, const char *, f32, s32);

    void emitEffectWithParticleCallBack(LiveActor *, const char *, MultiEmitterParticleCallBack *);
};