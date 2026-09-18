#pragma once

#include "JSystem/JGeometry/TMatrix.hpp"
#include <revolution/types.h>

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
        void checkEffectSceneUpdate(const EffectSystem* pSystem);
        void movementEffectNormal(const EffectSystem* pSystem);
        void drawEffect3D(const EffectSystem* pSystem, const TPos3f& rViewMtx);
        void drawEffect2D(const EffectSystem* pSystem);
        void forceDeleteAllEmitters(const EffectSystem* pSystem);
        bool isExistInResource(u16* pIndex, const char* pName);
        bool isEffect2D(const MultiEmitter* pEmitter);
        void initEffectSyncBck(EffectKeeper* pKeeper, const ModelManager* pModelManager, const char* pEffectName, const char* pAnimName,
                               s32 animCount, f32 startFrame, f32 endFrame, bool deleteOnEnd);
        void addEffectSyncBck(MultiEmitter* pEmitter, const ModelManager* pModelManager, const char* pAnimName);
        int getAutoEffectNum(const char* pName);
        JMapInfo* getAutoEffectListBinary();
        void setupMultiEmitter(EffectKeeper* pKeeper, const ModelManager* pModelManager, const AutoEffectInfo* pInfo);
        void setupMultiEmitterSyncBck(EffectKeeper* pKeeper, const ModelManager* pModelManager, const AutoEffectInfo* pInfo);
        void registerAutoEffectInfoGroup(EffectKeeper* pKeeper, const LiveActor* pActor, const char* pName);
        void requestMovementOn(EffectKeeper* pKeeper);
        void registerAutoEffectInfoGroup(PaneEffectKeeper* pKeeper, const LayoutActor* pActor, const char* pName);
        void registerAutoEffectInfoGroup(PaneEffectKeeper* pKeeper, const EffectSystem* pSystem, const LayoutActor* pActor, const char* pName);
        void addAutoEffect(EffectKeeper* pKeeper, const LiveActor* pActor, const AutoEffectInfo* pInfo);
        void addAutoEffect(PaneEffectKeeper* pKeeper, const LayoutActor* pActor, const AutoEffectInfo* pInfo);
        void addAutoEffect(MultiSceneEffectKeeper* pKeeper, const MultiSceneActor* pActor, const AutoEffectInfo* pInfo);
        void registerAutoEffectInfoGroup(MultiSceneEffectKeeper* pKeeper, const EffectSystem* pSystem, const MultiSceneActor* pActor,
                                         const char* pName);
        void deleteParticleEmitter(ParticleEmitter* pEmitter);
        void setLinkSingleEmitter(ParticleEmitter* pEmitter, SingleEmitter* pSingleEmitter);
        SingleEmitter* getLinkSingleEmitter(const JPABaseEmitter* pEmitter);
        void forceDeleteAllOneTimeEmitter();
        void createParticleEmitter(ParticleEmitter* pEmitter, JPAEmitterManager* pManager, const TVec3f& rPosition, u16 resourceId, u8 groupId,
                                   u8 resourceManagerId);
        bool isExistInResource(u16* pIndex, const char* pName, s32 number);
        const char* getEffectAttributeName(s32 floorCode);
        AutoEffectInfo* createAutoEffect(const char* pGroupName, const char* pUniqueName);
    };  // namespace Effect
};  // namespace MR
