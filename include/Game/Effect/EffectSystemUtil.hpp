#pragma once

#include "JSystem/JGeometry/TMatrix.hpp"
#include <revolution.h>

class AutoEffectInfo;
class EffectKeeper;
class EffectSystem;
class JMapInfo;
class JPABaseEmitter;
class JPAEmitterManager;
class LayoutActor;
class LiveActor;
class ModelManager;
class MultiEmitter;
class MultiSceneActor;
class MultiSceneEffectKeeper;
class PaneEffectKeeper;
class ParticleEmitter;
class SingleEmitter;

namespace MR {
    namespace Effect {
        void requestMovementOnAllEmitters();
        void requestMovementOffAllLoopEmitters();
        void checkEffectSceneUpdate(const EffectSystem*);
        void movementEffectNormal(const EffectSystem*);
        void drawEffect3D(const EffectSystem*, const TPos3f&);
        void drawEffect2D(const EffectSystem*);
        void forceDeleteAllEmitters(const EffectSystem*);
        bool isExistInResource(u16*, const char*);
        bool isEffect2D(const MultiEmitter*);
        void initEffectSyncBck(EffectKeeper*, const ModelManager*, const char*, const char*, s32, float, float, bool);
        void addEffectSyncBck(MultiEmitter*, const ModelManager*, const char*);
        int getAutoEffectNum(const char*);
        JMapInfo* getAutoEffectListBinary();
        void setupMultiEmitter(EffectKeeper*, const ModelManager*, const AutoEffectInfo*);
        void setupMultiEmitterSyncBck(EffectKeeper*, const ModelManager*, const AutoEffectInfo*);
        void registerAutoEffectInfoGroup(EffectKeeper*, const LiveActor*, const char*);
        void requestMovementOn(EffectKeeper*);
        void registerAutoEffectInfoGroup(PaneEffectKeeper*, const LayoutActor*, const char*);
        void registerAutoEffectInfoGroup(PaneEffectKeeper*, const EffectSystem*, const LayoutActor*, const char*);
        void addAutoEffect(EffectKeeper*, const LiveActor*, const AutoEffectInfo*);
        void addAutoEffect(PaneEffectKeeper*, const LayoutActor*, const AutoEffectInfo*);
        void addAutoEffect(MultiSceneEffectKeeper*, const MultiSceneActor*, const AutoEffectInfo*);
        void registerAutoEffectInfoGroup(MultiSceneEffectKeeper*, const EffectSystem*, const MultiSceneActor*, const char*);
        void deleteParticleEmitter(ParticleEmitter*);
        void setLinkSingleEmitter(ParticleEmitter*, SingleEmitter*);
        SingleEmitter* getLinkSingleEmitter(const JPABaseEmitter*);
        void forceDeleteAllOneTimeEmitter();
        void createParticleEmitter(ParticleEmitter*, JPAEmitterManager*, const TVec3f&, u16, u8, u8);
        bool isExistInResource(u16*, const char*, s32);
        const char* getEffectAttributeName(s32);
        AutoEffectInfo* createAutoEffect(const char*, const char*);
    };  // namespace Effect
};  // namespace MR
