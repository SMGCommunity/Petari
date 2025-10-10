#include "Game/Util/EffectUtil.hpp"
#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/Effect/MultiEmitter.hpp"
#include "Game/Effect/MultiEmitterParticleCallBack.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/PaneEffectKeeper.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include <JSystem/JMath/JMath.hpp>

namespace {
    bool isExistEffect(const LiveActor *pActor, const char *pEffectName) {
        return MR::isExistEffectKeeper(pActor)
            && MR::isRegisteredEffect(pActor, pEffectName);
    }
};

namespace MR {
    void requestEffectStopSceneStart() {
        Effect::requestMovementOffAllLoopEmitters();
    }

    void requestEffectStopSceneEnd() {
        Effect::requestMovementOnAllEmitters();
    }

    void addEffect(LiveActor *pActor, const char *pEffectName) {
        pActor->mEffectKeeper->addEffect(pEffectName, pActor);
    }

    MultiEmitter* getEffect(const LiveActor *pActor, const char *pEffectName) {
        return pActor->mEffectKeeper->getEmitter(pEffectName);
    }

    MultiEmitter* getEffect(const LayoutActor *pLayoutActor, const char *pEffectName) {
        return pLayoutActor->mPaneEffectKeeper->getEmitter(pEffectName);
    }

    bool isExistEffectKeeper(const LiveActor *pActor) {
        return pActor->mEffectKeeper;
    }

    bool isExistEffectKeeper(const LayoutActor *pLayoutActor) {
        return pLayoutActor->mPaneEffectKeeper;
    }

    MultiEmitter* emitEffect(LiveActor *pActor, const char *pEffectName) {
        return pActor->mEffectKeeper->createEmitter(pEffectName);
    }

    MultiEmitter* emitEffectWithScale(LiveActor *pActor, const char *pEffectName, f32 a3, s32 a4) {
        MultiEmitter* pEmitter = pActor->mEffectKeeper->createEmitter(pEffectName);
        TVec3f scaleVec(a3,a3,a3);
        pEmitter->setGlobalScale(scaleVec, a4);
        return pEmitter;
    }

    MultiEmitter* emitEffectWithEmitterCallBack(LiveActor *pActor, const char *pEffectName, MultiEmitterCallBackBase *pCallBack) {
        MultiEmitter* pEmitter = pActor->mEffectKeeper->getEmitter(pEffectName);
        pEmitter->createEmitterWithCallBack(pCallBack);
        return pEmitter;
    }

    MultiEmitter* emitEffectWithParticleCallBack(LiveActor *pActor, const char *pEffectName, MultiEmitterParticleCallBack *pCallBack) {
        MultiEmitter* pEmitter = pActor->mEffectKeeper->createEmitter(pEffectName);
        pEmitter->setParticleCallBackPtr(pCallBack, -1);
        return pEmitter;
    }

    MultiEmitter* tryEmitEffect(LiveActor *pActor, const char *pEffectName) {
        if (::isExistEffect(pActor, pEffectName) == false) {
            return nullptr;
        }
        else {
            return pActor->mEffectKeeper->createEmitter(pEffectName);
        }
    }

    bool tryDeleteEffect(LiveActor *pActor, const char *pEffectName) {
        if (!::isExistEffect(pActor, pEffectName)) {
            return false;
        }
        else {
            pActor->mEffectKeeper->deleteEmitter(pEffectName);
            return true;
        }
    }

    void deleteEffect(LiveActor *pActor, const char *pEffectName) {
        pActor->mEffectKeeper->deleteEmitter(pEffectName);
    }

    void forceDeleteEffect(LiveActor *pActor, const char *pEffectName) {
        pActor->mEffectKeeper->forceDeleteEmitter(pEffectName);
    }

    void deleteEffectAll(LiveActor *pActor) {
        pActor->mEffectKeeper->deleteEmitterAll();
    }

    void forceDeleteEffectAll(LiveActor *pActor) {
        pActor->mEffectKeeper->forceDeleteEmitterAll();
    }

    bool isRegisteredEffect(const LiveActor *pActor, const char *pEffectName) {
        if (pEffectName) {
            return pActor->mEffectKeeper->isRegisteredEmitter(pEffectName);
        }
        else return pActor->mEffectKeeper;
    }

    bool isEffectValid(const LiveActor *pActor, const char *pEffectName) {
        MultiEmitter* pEmitter = pActor->mEffectKeeper->getEmitter(pEffectName);
        if (pEmitter) {
            return pEmitter->isValid();
        }
        else {
            return false;
        }
    }

    void onDrawEffect(LiveActor *pActor) {
        pActor->mEffectKeeper->onDraw();
    }

    void offDrawEffect(LiveActor *pActor) {
        pActor->mEffectKeeper->offDraw();
    }

    void pauseOffEffectAll(LiveActor *pActor) {
        Effect::requestMovementOn(pActor->mEffectKeeper);
    }

    void onEmitEffectSyncClipping(LiveActor *pActor, const char *pEffectName) {
        pActor->mEffectKeeper->getEmitter(pEffectName)->onCreateSyncClipping();
    }

    void onForceDeleteEffectSyncClipping(LiveActor *pActor, const char *pEffectName) {
        pActor->mEffectKeeper->getEmitter(pEffectName)->onForceDeleteSyncClipping();
    }

    void setEffectName(LiveActor *pActor, const char *pOldName, const char *pNewName) {
        pActor->mEffectKeeper->changeEffectName(pOldName, pNewName);
    }

    void setEffectHostSRT(LiveActor *pActor, const char *pEffectName, const TVec3f *a3, const TVec3f *a4, const TVec3f *a5) {
        pActor->mEffectKeeper->getEmitter(pEffectName)->setHostSRT(a3, a4, a5);
    }

    void setEffectHostMtx(LiveActor *pActor, const char *pEffectName, MtxPtr mtx) {
        pActor->mEffectKeeper->getEmitter(pEffectName)->setHostMtx(mtx);
    }

    void setEffectBaseScale(LiveActor *pActor, const char *pEffectName, f32 scale) {
        pActor->mEffectKeeper->getEmitter(pEffectName)->setBaseScale(scale);
    }
    
    void setEffectLocalScale(LiveActor *pActor, const char *pEffectName, const TVec3f &rScaleVec) {
        pActor->mEffectKeeper->getEmitter(pEffectName)->setLocalScale(rScaleVec, -1);
    }

    void setEffectColor(LiveActor *pActor, const char *pEffectName, u8 colorPrm1, u8 colorPrm2, u8 colorPrm3, u8 colorEnv1, u8 colorEnv2, u8 colorEnv3) {
        pActor->mEffectKeeper->getEmitter(pEffectName)->setGlobalPrmColor(colorPrm1, colorPrm2, colorPrm3, -1);
        pActor->mEffectKeeper->getEmitter(pEffectName)->setGlobalEnvColor(colorEnv1, colorEnv2, colorEnv3, -1);
    }

    void setEffectPrmColor(LiveActor *pActor, const char *pEffectName, u8 color1, u8 color2, u8 color3) {
        pActor->mEffectKeeper->getEmitter(pEffectName)->setGlobalPrmColor(color1, color2, color3, -1);
    }

    void setEffectEnvColor(LiveActor *pActor, const char *pEffectName, u8 color1, u8 color2, u8 color3) {
        pActor->mEffectKeeper->getEmitter(pEffectName)->setGlobalEnvColor(color1, color2, color3, -1);
    }

    void emitEffectHit(LiveActor *pActor, const TVec3f &a2, const char *pEffectName) {
        if (!pEffectName) {
            pEffectName = "HitMarkNormal";
        }
        pActor->mEffectKeeper->createEmitter(pEffectName)->setGlobalTranslation(a2, -1);
    }

    void emitEffectHit(LiveActor *pActor, const TVec3f &a2, const TVec3f &a3, const char *pEffectName) {
        if (!pEffectName) {
            pEffectName = "HitMarkNormal";
        }
        TPos3f SRTMatrix;
        makeMtxUpNoSupportPos(&SRTMatrix, a3, a2);
        pActor->mEffectKeeper->createEmitter(pEffectName)->setGlobalSRTMatrix(SRTMatrix, -1);
    }

    void emitEffectHit(LiveActor *pActor, MtxPtr mtx, const char *pEffectName) {
        if (!pEffectName) {
            pEffectName = "HitMarkNormal";
        }
        pActor->mEffectKeeper->createEmitter(pEffectName)->setGlobalSRTMatrix(mtx, -1);
    }

    void emitEffectHitBetweenSensors(LiveActor *pActor, const HitSensor *pSensor1, const HitSensor *pSensor2, f32 flt, const char *pEffectName) {
        TVec3f vecBetween;
        calcPosBetweenSensors(&vecBetween, pSensor1, pSensor2, flt);
        emitEffectHit(pActor, vecBetween, pEffectName);
    }

    void initEffectAfterPlacement(LiveActor *pActor) {
        EffectKeeper* pEffectKeeper = pActor->mEffectKeeper;
        pEffectKeeper->initAfterPlacementForAttributeEffect(pActor->getBaseMtx());
    }

    void updateEffectFloorCode(LiveActor *pActor, const Triangle *pTriangle) {
        pActor->mEffectKeeper->updateFloorCode(pTriangle);
    }

    void updateEffectFloorCodeLineToMap(LiveActor *pActor, const TVec3f &a2, const TVec3f &a3) {
        Triangle tri = Triangle();
        if (getFirstPolyOnLineToMap(nullptr, &tri, a2, a3)) {
            pActor->mEffectKeeper->updateFloorCode(&tri);
        }
        else {
            pActor->mEffectKeeper->updateFloorCode(nullptr);
        }
    }

    // Regswap + stackswap
    /* void updateEffectFloorCodeLineToMap(LiveActor *pActor, f32 a2, f32 a3) {
        TVec3f gravity(pActor->mGravity);
        TVec3f scaledGrav = gravity * a3;
        TVec3f sumPosScaledGrav(pActor->mPosition);
        JMathInlineVEC::PSVECAdd2((Vec*)&scaledGrav, (Vec*)&sumPosScaledGrav, (Vec*)&sumPosScaledGrav);
        TVec3f scaledGrav2 = gravity * (a2-a3);
        updateEffectFloorCodeLineToMap(pActor, sumPosScaledGrav, scaledGrav2);
    } */

    // Not byte-matching but does the same thing
    void initEffectSyncBck(LiveActor *pActor, const char *pEffectName, const char* const *pStrList) {
        int i = 0;
        const char* const *tempStrList = pStrList;
        const char* str;
        while (str = tempStrList[0], tempStrList = &tempStrList[1], str) {
            i++;
        }
        Effect::initEffectSyncBck(pActor->mEffectKeeper, pActor->mModelManager, pEffectName, pStrList[0], i, 0.0f, -1.0f, false);
        pStrList = &pStrList[1];
        ModelManager* pModelManager;
        while (pStrList[0]) {
            pModelManager = pActor->mModelManager;
            Effect::addEffectSyncBck(pActor->mEffectKeeper->getEmitter(pEffectName), pModelManager, pStrList[0]);
            pStrList = &pStrList[1];
        }
    }

    // Instructionswap for some reason
    void addEffectHitNormal(LiveActor *pActor, const char *pEffectName) {
        pActor->mEffectKeeper->addEffect("HitMarkNormal", pActor);
        if (pEffectName) {
            pActor->mEffectKeeper->changeEffectName("HitMarkNormal", pEffectName);
        }
    }
};
