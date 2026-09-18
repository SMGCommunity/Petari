#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/Effect/AutoEffectGroupHolder.hpp"
#include "Game/Effect/AutoEffectInfo.hpp"
#include "Game/Effect/EffectSystem.hpp"
#include "Game/Effect/MultiEmitter.hpp"
#include "Game/Effect/ParticleCalcExecutor.hpp"
#include "Game/Effect/ParticleDrawExecutor.hpp"
#include "Game/Effect/ParticleEmitter.hpp"
#include "Game/Effect/ParticleEmitterHolder.hpp"
#include "Game/Effect/ParticleResourceHolder.hpp"
#include "Game/LiveActor/EffectKeeper.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelManager.hpp"
#include "Game/Scene/MultiSceneActor.hpp"
#include "Game/Scene/MultiSceneEffectKeeper.hpp"
#include "Game/Screen/PaneEffectKeeper.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <cstdio>
#include <cstring>

void EffectSystemUtil_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
}

namespace {
    s32 countAnimNames(const char* pNames) {
        s32 count = 0;
        for (u32 i = 0; i <= strlen(pNames); i++) {
            if (pNames[i] == ' ' || pNames[i] == '\0') {
                count++;
            }
        }

        return count;
    }

    void setupMultiEmitter(MultiEmitter* pEmitter, const AutoEffectInfo* pInfo) {
        pEmitter->_28 = pInfo;
        pEmitter->setDrawOrder(pInfo->mDrawOrder);
        pEmitter->setOffset(pInfo->mOffset);

        if (!MR::isNearZero(pInfo->mScaleValue - 1.0f)) {
            pEmitter->setBaseScale(pInfo->mScaleValue);
        }

        if (!MR::isNearZero(pInfo->mRateValue - 1.0f)) {
            pEmitter->setRate(pInfo->mRateValue, -1);
        }

        if (pInfo->mIsValidPrmColor) {
            Color8 color = pInfo->mPrmColor;
            pEmitter->setGlobalPrmColor(color.r, color.g, color.b, -1);
        }

        if (pInfo->mIsValidEnvColor) {
            Color8 color = pInfo->mEnvColor;
            pEmitter->setGlobalEnvColor(color.r, color.g, color.b, -1);
        }

        if (!MR::isNearZero(pInfo->mLightAffectValue)) {
            pEmitter->_2C = pInfo->mLightAffectValue;
        }
    }
}  // namespace

namespace MR {
    namespace Effect {
        void requestMovementOnAllEmitters() {
            getEffectSystem()->mCalcExec->requestMovementOnPauseIgnore();
            getEffectSystem()->mEmitterHolder->requestMovementOnAllEmitters();
        }

        void requestMovementOffAllLoopEmitters() {
            getEffectSystem()->mEmitterHolder->requestMovementOffAllLoopEmitters();
        }

        void checkEffectSceneUpdate(const EffectSystem* pSystem) {
            pSystem->mCalcExec->movementCheckUpdate();
        }

        void movementEffectNormal(const EffectSystem* pSystem) {
            pSystem->mCalcExec->movementNormal();
        }

        void drawEffect3D(const EffectSystem* pSystem, const TPos3f& rViewMtx) {
            pSystem->mDrawExec->drawWithViewMtx3D(rViewMtx);
        }

        void drawEffect2D(const EffectSystem* pSystem) {
            pSystem->mDrawExec->draw2D();
        }

        void forceDeleteAllEmitters(const EffectSystem* pSystem) {
            pSystem->mEmitterHolder->forceDeleteAllEmitters();
        }

        bool isExistInResource(u16* pIndex, const char* pName) {
            return getParticleResourceHolder()->isExistInResource(pName, pIndex);
        }

        bool isEffect2D(const MultiEmitter* pEmitter) {
            if (pEmitter->_28 == nullptr) {
                return false;
            }

            return pEmitter->_28->mDrawOrder == 6 || pEmitter->_28->mDrawOrder == 7;
        }

        void initEffectSyncBck(EffectKeeper* pKeeper, const ModelManager* pModelManager, const char* pEffectName, const char* pAnimName,
                               s32 animCount, f32 startFrame, f32 endFrame, bool deleteOnEnd) {
            pKeeper->registerSyncBckEffect(pModelManager->mXanimePlayer, pEffectName, pAnimName, animCount, startFrame, endFrame, deleteOnEnd);
        }

        void addEffectSyncBck(MultiEmitter* pEmitter, const ModelManager* pModelManager, const char* pAnimName) {
            pEmitter->addSyncBck(pModelManager->mXanimePlayer, pAnimName);
        }

        int getAutoEffectNum(const char* pName) {
            return getParticleResourceHolder()->getAutoEffectNum(pName);
        }

        JMapInfo* getAutoEffectListBinary() {
            return getParticleResourceHolder()->getAutoEffectListBinary();
        }

        void setupMultiEmitter(EffectKeeper* pKeeper, const ModelManager* pModelManager, const AutoEffectInfo* pInfo) {
            MultiEmitter* pEmitter = pKeeper->getEmitter(pInfo->getName());
            setupMultiEmitterSyncBck(pKeeper, pModelManager, pInfo);
            ::setupMultiEmitter(pEmitter, pInfo);
            pEmitter->scanParticleEmitter(getEffectSystem());

            if (pInfo->mParentName != nullptr) {
                pKeeper->getEmitter(pInfo->mParentName)->addChildEmitter(pEmitter);
            }
        }

        void setupMultiEmitterSyncBck(EffectKeeper* pKeeper, const ModelManager* pModelManager, const AutoEffectInfo* pInfo) {
            if (pInfo->mAnimName == nullptr) {
                return;
            }

            MultiEmitter* pEmitter = pKeeper->getEmitter(pInfo->getName());
            if (!hasStringSpace(pInfo->mAnimName)) {
                const char* pAnimName = pInfo->mAnimName;
                initEffectSyncBck(pKeeper, pModelManager, pInfo->getName(), pAnimName, 1, pInfo->mStartFrame, pInfo->mEndFrame, false);
            } else {
                const char* const pAnimNames = pInfo->mAnimName;
                char name[256];
                zeroMemory(name, sizeof(name));
                s32 nameLength = 0;
                for (u32 i = 0; i <= strlen(pAnimNames); i++) {
                    if (pAnimNames[i] == ' ' || pAnimNames[i] == '\0') {
                        name[nameLength] = '\0';
                        const char* pAnimName = nullptr;
                        findBckNameStringInResource(&pAnimName, pModelManager->getResourceHolder(), name);
                        if (pEmitter->_24 == nullptr) {
                            initEffectSyncBck(pKeeper, pModelManager, pInfo->getName(), pAnimName, countAnimNames(pAnimNames), 0.0f, -1.0f, false);
                        } else {
                            addEffectSyncBck(pEmitter, pModelManager, pAnimName);
                        }

                        nameLength = 0;
                        zeroMemory(name, sizeof(name));
                    } else {
                        name[nameLength] = pAnimNames[i];
                        nameLength++;
                    }
                }
            }

            pEmitter->setContinueBckEnd(!((pInfo->mFlag & AutoEffectInfo::FLAG_CONTINUE_ANIM_END) - AutoEffectInfo::FLAG_CONTINUE_ANIM_END));
        }

        void registerAutoEffectInfoGroup(EffectKeeper* pKeeper, const LiveActor* pActor, const char* pName) {
            createAndAddAutoEffectGroup(getEffectSystem()->mGroupHolder, pName);
            registerAutoEffectInfos(getEffectSystem()->mGroupHolder, pKeeper, pActor, pName);
        }

        void requestMovementOn(EffectKeeper* pKeeper) {
            for (s32 i = 0; i < pKeeper->_C.size(); i++) {
                MultiEmitter* pEmitter = pKeeper->getEmitter(i);
                if (pEmitter != nullptr && pEmitter->isValid()) {
                    pEmitter->pauseOff(-1);
                }
            }
        }

        void registerAutoEffectInfoGroup(PaneEffectKeeper* pKeeper, const LayoutActor* pActor, const char* pName) {
            registerAutoEffectInfoGroup(pKeeper, getEffectSystem(), pActor, pName);
        }

        void registerAutoEffectInfoGroup(PaneEffectKeeper* pKeeper, const EffectSystem* pSystem, const LayoutActor* pActor, const char* pName) {
            createAndAddAutoEffectGroup(pSystem->mGroupHolder, pName);
            registerAutoEffectInfos(pSystem->mGroupHolder, pKeeper, pActor, pName);
        }

        void addAutoEffect(EffectKeeper* pKeeper, const LiveActor* pActor, const AutoEffectInfo* pInfo) {
            if (pInfo->mJointName != nullptr) {
                const char* pEffectName = pInfo->mEffectName;
                pKeeper->registerEffect(pEffectName, getJointMtx(pActor, pInfo->mJointName), pInfo->mUniqueName, nullptr);
            } else if (pActor->getBaseMtx() != nullptr) {
                const char* pEffectName = pInfo->mEffectName;
                pKeeper->registerEffect(pEffectName, pActor->getBaseMtx(), &pActor->mScale, pInfo->mUniqueName, nullptr);
            } else {
                pKeeper->registerEffect(pInfo->mEffectName, &pActor->mPosition, &pActor->mRotation, &pActor->mScale, pInfo->mUniqueName);
            }

            setupMultiEmitter(pKeeper, pActor->mModelManager, pInfo);
        }

        void addAutoEffect(PaneEffectKeeper* pKeeper, const LayoutActor* pActor, const AutoEffectInfo* pInfo) {
            pKeeper->add(pInfo->mJointName, pInfo->mEffectName, pInfo->mUniqueName);
            ::setupMultiEmitter(pKeeper->getEmitter(pInfo->mUniqueName), pInfo);
        }

        void addAutoEffect(MultiSceneEffectKeeper* pKeeper, const MultiSceneActor* pActor, const AutoEffectInfo* pInfo) {
            if (pInfo->mJointName != nullptr) {
                const char* pEffectName = pInfo->mEffectName;
                pKeeper->add(pEffectName, MultiScene::getJointMtx(pActor, pInfo->mJointName), pInfo->mUniqueName);
            } else {
                pKeeper->add(pInfo->mEffectName, &pActor->mTranslation, &pActor->mRotation, &pActor->mScale, pInfo->mUniqueName);
            }

            ::setupMultiEmitter(pKeeper->get(pInfo->mUniqueName), pInfo);
        }

        void registerAutoEffectInfoGroup(MultiSceneEffectKeeper* pKeeper, const EffectSystem* pSystem, const MultiSceneActor* pActor,
                                         const char* pName) {
            createAndAddAutoEffectGroup(pSystem->mGroupHolder, pName);
            registerAutoEffectInfos(pSystem->mGroupHolder, pKeeper, pActor, pName);
        }

        void deleteParticleEmitter(ParticleEmitter* pEmitter) {
            if (pEmitter->mEmitter != nullptr) {
                pEmitter->mEmitter->playCalcEmitter();
                pEmitter->mEmitter->becomeInvalidEmitter();
            }
        }

        void setLinkSingleEmitter(ParticleEmitter* pEmitter, SingleEmitter* pSingleEmitter) {
            pEmitter->mEmitter->setUserWork(reinterpret_cast< uintptr_t >(pSingleEmitter));
        }

        SingleEmitter* getLinkSingleEmitter(const JPABaseEmitter* pEmitter) {
            return reinterpret_cast< SingleEmitter* >(pEmitter->getUserWork());
        }

        void forceDeleteAllOneTimeEmitter() {
            getEffectSystem()->mEmitterHolder->forceDeleteAllOneTimeEmitters();
        }

        void createParticleEmitter(ParticleEmitter* pEmitter, JPAEmitterManager* pManager, const TVec3f& rPosition, u16 resourceId, u8 groupId,
                                   u8 resourceManagerId) {
            JPABaseEmitter* pBaseEmitter = pManager->createSimpleEmitterID(rPosition, resourceId, groupId, resourceManagerId, nullptr, nullptr);
            if (pBaseEmitter != nullptr) {
                pEmitter->mEmitter = pBaseEmitter;
                pEmitter->init(resourceId);
            }
        }

        bool isExistInResource(u16* pIndex, const char* pName, s32 number) {
            char name[42];
            snprintf(name, sizeof(name), "%s%02d", pName, number);
            return isExistInResource(pIndex, name);
        }

        const char* getEffectAttributeName(s32 floorCode) {
            switch (floorCode) {
            case 20:
            case 21:
            case 22:
            case 23:
                return "Water";
            case 13:
            case 30:
                return "Sand";
            case 5:
                return "Ice";
            case 10:
                return "DamageFire";
            case 31:
            case 32:
                return "Mud";
            default:
                return "Default";
            }
        }

        AutoEffectInfo* createAutoEffect(const char* pGroupName, const char* pUniqueName) {
            JMapInfo* pInfo = getAutoEffectListBinary();
            JMapInfoIter iter = pInfo->findElement("UniqueName", pUniqueName, 0);
            const char* pName = "";
            iter.getValue("GroupName", &pName);
            AutoEffectInfo* pEffectInfo = new AutoEffectInfo();
            pEffectInfo->init(iter);
            return pEffectInfo;
        }
    }  // namespace Effect
}  // namespace MR
