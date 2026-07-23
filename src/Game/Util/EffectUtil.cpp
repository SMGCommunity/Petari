#include "Game/Util/EffectUtil.hpp"
#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/Effect/MultiEmitter.hpp"
#include "Game/Effect/MultiEmitterParticleCallBack.hpp"
#include "Game/LiveActor/EffectKeeper.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/PaneEffectKeeper.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MtxUtil.hpp"

namespace {
    EffectKeeper* getEffectKeeper(const LiveActor* pActor) {
        return pActor->mEffectKeeper;
    }

    bool isExistEffect(const LiveActor* pActor, const char* pEffectName) {
        return MR::isExistEffectKeeper(pActor) && MR::isRegisteredEffect(pActor, pEffectName);
    }
};  // namespace

namespace MR {
    void requestEffectStopSceneStart() {
        Effect::requestMovementOffAllLoopEmitters();
    }

    void requestEffectStopSceneEnd() {
        Effect::requestMovementOnAllEmitters();
    }

    void addEffect(LiveActor* pActor, const char* pEffectName) {
        ::getEffectKeeper(pActor)->addEffect(pEffectName, pActor);
    }

    MultiEmitter* getEffect(const LiveActor* pActor, const char* pEffectName) {
        return ::getEffectKeeper(pActor)->getEmitter(pEffectName);
    }

    MultiEmitter* getEffect(const LayoutActor* pActor, const char* pEffectName) {
        return pActor->mEffectKeeper->getEmitter(pEffectName);
    }

    bool isExistEffectKeeper(const LiveActor* pActor) {
        return ::getEffectKeeper(pActor) != nullptr;
    }

    bool isExistEffectKeeper(const LayoutActor* pActor) {
        return pActor->mEffectKeeper != nullptr;
    }

    MultiEmitter* emitEffect(LiveActor* pActor, const char* pEffectName) {
        return ::getEffectKeeper(pActor)->createEmitter(pEffectName);
    }

    MultiEmitter* emitEffectWithScale(LiveActor* pActor, const char* pEffectName, f32 a3, s32 a4) {
        MultiEmitter* pEmitter;

        pEmitter = emitEffect(pActor, pEffectName);
        pEmitter->setGlobalScale(TVec3f(a3, a3, a3), a4);

        return pEmitter;
    }

    MultiEmitter* emitEffectWithEmitterCallBack(LiveActor* pActor, const char* pEffectName, MultiEmitterCallBackBase* pCallBack) {
        MultiEmitter* pEmitter;

        pEmitter = getEffect(pActor, pEffectName);
        pEmitter->createEmitterWithCallBack(pCallBack);

        return pEmitter;
    }

    MultiEmitter* emitEffectWithParticleCallBack(LiveActor* pActor, const char* pEffectName, MultiEmitterParticleCallBack* pCallBack) {
        MultiEmitter* pEmitter;

        pEmitter = emitEffect(pActor, pEffectName);
        pEmitter->setParticleCallBackPtr(pCallBack, -1);

        return pEmitter;
    }

    MultiEmitter* tryEmitEffect(LiveActor* pActor, const char* pEffectName) {
        if (::isExistEffect(pActor, pEffectName) == false) {
            return nullptr;
        }

        return emitEffect(pActor, pEffectName);
    }

    bool tryDeleteEffect(LiveActor* pActor, const char* pEffectName) {
        if (::isExistEffect(pActor, pEffectName) == false) {
            return false;
        }

        ::getEffectKeeper(pActor)->deleteEmitter(pEffectName);

        return true;
    }

    void deleteEffect(LiveActor* pActor, const char* pEffectName) {
        ::getEffectKeeper(pActor)->deleteEmitter(pEffectName);
    }

    void forceDeleteEffect(LiveActor* pActor, const char* pEffectName) {
        ::getEffectKeeper(pActor)->forceDeleteEmitter(pEffectName);
    }

    void deleteEffectAll(LiveActor* pActor) {
        ::getEffectKeeper(pActor)->deleteEmitterAll();
    }

    void forceDeleteEffectAll(LiveActor* pActor) {
        ::getEffectKeeper(pActor)->forceDeleteEmitterAll();
    }

    bool isRegisteredEffect(const LiveActor* pActor, const char* pEffectName) {
        if (pEffectName != nullptr) {
            return ::getEffectKeeper(pActor)->isRegisteredEmitter(pEffectName);
        } else {
            return isExistEffectKeeper(pActor);
        }
    }

    bool isEffectValid(const LiveActor* pActor, const char* pEffectName) {
        MultiEmitter* pEmitter = getEffect(pActor, pEffectName);

        if (pEmitter != nullptr) {
            return pEmitter->isValid();
        } else {
            return false;
        }
    }

    void onDrawEffect(LiveActor* pActor) {
        ::getEffectKeeper(pActor)->onDraw();
    }

    void offDrawEffect(LiveActor* pActor) {
        ::getEffectKeeper(pActor)->offDraw();
    }

    void pauseOffEffectAll(LiveActor* pActor) {
        Effect::requestMovementOn(::getEffectKeeper(pActor));
    }

    void onEmitEffectSyncClipping(LiveActor* pActor, const char* pEffectName) {
        getEffect(pActor, pEffectName)->onCreateSyncClipping();
    }

    void onForceDeleteEffectSyncClipping(LiveActor* pActor, const char* pEffectName) {
        getEffect(pActor, pEffectName)->onForceDeleteSyncClipping();
    }

    void setEffectName(LiveActor* pActor, const char* pOldName, const char* pNewName) {
        ::getEffectKeeper(pActor)->changeEffectName(pOldName, pNewName);
    }

    void setEffectHostSRT(LiveActor* pActor, const char* pEffectName, const TVec3f* a3, const TVec3f* a4, const TVec3f* a5) {
        getEffect(pActor, pEffectName)->setHostSRT(a3, a4, a5);
    }

    void setEffectHostMtx(LiveActor* pActor, const char* pEffectName, MtxPtr mtx) {
        getEffect(pActor, pEffectName)->setHostMtx(mtx);
    }

    void setEffectBaseScale(LiveActor* pActor, const char* pEffectName, f32 scale) {
        getEffect(pActor, pEffectName)->setBaseScale(scale);
    }

    void setEffectLocalScale(LiveActor* pActor, const char* pEffectName, const TVec3f& rScaleVec) {
        getEffect(pActor, pEffectName)->setLocalScale(rScaleVec, -1);
    }

    void setEffectColor(LiveActor* pActor, const char* pEffectName, u8 colorPrm1, u8 colorPrm2, u8 colorPrm3, u8 colorEnv1, u8 colorEnv2,
                        u8 colorEnv3) {
        getEffect(pActor, pEffectName)->setGlobalPrmColor(colorPrm1, colorPrm2, colorPrm3, -1);
        getEffect(pActor, pEffectName)->setGlobalEnvColor(colorEnv1, colorEnv2, colorEnv3, -1);
    }

    void setEffectPrmColor(LiveActor* pActor, const char* pEffectName, u8 color1, u8 color2, u8 color3) {
        getEffect(pActor, pEffectName)->setGlobalPrmColor(color1, color2, color3, -1);
    }

    void setEffectEnvColor(LiveActor* pActor, const char* pEffectName, u8 color1, u8 color2, u8 color3) {
        getEffect(pActor, pEffectName)->setGlobalEnvColor(color1, color2, color3, -1);
    }

    void emitEffectHit(LiveActor* pActor, const TVec3f& a2, const char* pEffectName) {
        if (pEffectName == nullptr) {
            pEffectName = "HitMarkNormal";
        }

        emitEffect(pActor, pEffectName)->setGlobalTranslation(a2, -1);
    }

    void emitEffectHit(LiveActor* pActor, const TVec3f& a2, const TVec3f& a3, const char* pEffectName) {
        if (pEffectName == nullptr) {
            pEffectName = "HitMarkNormal";
        }

        TPos3f mtx;
        makeMtxUpNoSupportPos(&mtx, a3, a2);
        emitEffect(pActor, pEffectName)->setGlobalSRTMatrix(mtx, -1);
    }

    void emitEffectHit(LiveActor* pActor, MtxPtr mtx, const char* pEffectName) {
        if (pEffectName == nullptr) {
            pEffectName = "HitMarkNormal";
        }

        emitEffect(pActor, pEffectName)->setGlobalSRTMatrix(mtx, -1);
    }

    void emitEffectHitBetweenSensors(LiveActor* pActor, const HitSensor* pSensor1, const HitSensor* pSensor2, f32 flt, const char* pEffectName) {
        TVec3f vecBetween;
        calcPosBetweenSensors(&vecBetween, pSensor1, pSensor2, flt);
        emitEffectHit(pActor, vecBetween, pEffectName);
    }

    void initEffectAfterPlacement(LiveActor* pActor) {
        ::getEffectKeeper(pActor)->initAfterPlacementForAttributeEffect(pActor->getBaseMtx());
    }

    void updateEffectFloorCode(LiveActor* pActor, const Triangle* pTriangle) {
        ::getEffectKeeper(pActor)->updateFloorCode(pTriangle);
    }

    void updateEffectFloorCodeLineToMap(LiveActor* pActor, const TVec3f& a2, const TVec3f& a3) {
        Triangle triangle = Triangle();

        if (getFirstPolyOnLineToMap(nullptr, &triangle, a2, a3)) {
            ::getEffectKeeper(pActor)->updateFloorCode(&triangle);
        } else {
            ::getEffectKeeper(pActor)->updateFloorCode(nullptr);
        }
    }

    void updateEffectFloorCodeLineToMap(LiveActor* pActor, f32 a2, f32 a3) {
        TVec3f gravity = pActor->mGravity;
        TVec3f sumPosScaledGrav = pActor->mPosition + gravity * a3;

        updateEffectFloorCodeLineToMap(pActor, sumPosScaledGrav, gravity * (a2 - a3));
    }

    void initEffectSyncBck(LiveActor* pActor, const char* pEffectName, const char* const* pStrList) {
        const char* str;
        const char* const* current = pStrList;

        int size = 0;
        while (str = *current, current++, str) {
            size++;
        }

        Effect::initEffectSyncBck(::getEffectKeeper(pActor), pActor->mModelManager, pEffectName, *pStrList, size, 0.0f, -1.0f, false);

        pStrList = &pStrList[1];
        while (*pStrList != nullptr) {
            ModelManager* pModelManager = pActor->mModelManager;
            MultiEmitter* pEmitter = ::getEffectKeeper(pActor)->getEmitter(pEffectName);
            Effect::addEffectSyncBck(pEmitter, pModelManager, *pStrList);
            pStrList++;
        }
    }

    void addEffectHitNormal(LiveActor* pActor, const char* pEffectName) {
        char* effectName = "HitMarkNormal";
        ::getEffectKeeper(pActor)->addEffect(effectName, pActor);

        if (pEffectName != nullptr) {
            setEffectName(pActor, effectName, pEffectName);
        }
    }
};  // namespace MR
