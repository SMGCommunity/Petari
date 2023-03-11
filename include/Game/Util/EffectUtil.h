#pragma once

#include <revolution.h>
#include <JSystem/JGeometry.h>

class ParticleEmitter;
class LiveActor;
class HitSensor;
class SingleEmitter;

namespace MR {
    namespace Effect {
        void deleteParticleEmitter(ParticleEmitter *);
        void setLinkSingleEmitter(ParticleEmitter *, SingleEmitter *);
    };

    bool isExistEffectKeeper(const LiveActor *);

    bool isRegisteredEffect(const LiveActor *, const char *);

    bool isEffectValid(const LiveActor *, const char *);
    void deleteEffect(LiveActor *, const char *);

    void deleteEffectAll(LiveActor *);

    void emitEffect(LiveActor *, const char *);

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
};