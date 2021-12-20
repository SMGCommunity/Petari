#include "Game/Util.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/ActorLightCtrl.h"
#include "Game/Map/LightFunction.h"

namespace MR {
    void calcAnimDirect(LiveActor *pActor) {
        bool isNoCalcAnim = pActor->mFlags.mIsNoCalcAnim;
        pActor->mFlags.mIsNoCalcAnim = false;
        pActor->calcAnim();

        if (isNoCalcAnim) {
            pActor->mFlags.mIsNoCalcAnim = true;
        }
    }

    void initLightCtrl(LiveActor *pActor) {
        pActor->initActorLightCtrl();
        pActor->mActorLightCtrl->init(-1, false);
    }

    void initLightCtrlForPlayer(LiveActor *pActor) {
        pActor->initActorLightCtrl();
        pActor->mActorLightCtrl->init(-1, false);
        LightFunction::registerPlayerLightCtrl(pActor->mActorLightCtrl);
    }

    void initLightCtrlNoDrawEnemy(LiveActor *pActor) {
        pActor->initActorLightCtrl();
        pActor->mActorLightCtrl->init(1, true);
    }

    void initLightCtrlNoDrawMapObj(LiveActor *pActor) {
        pActor->initActorLightCtrl();
        pActor->mActorLightCtrl->init(3, true);
    }

    void updateLightCtrl(LiveActor *pActor) {
        pActor->mActorLightCtrl->update(false);
    }

    void updateLightCtrlDirect(LiveActor *pActor) {
        pActor->mActorLightCtrl->update(true);
    }

    void loadActorLight(const LiveActor *pActor) {
        pActor->mActorLightCtrl->loadLight();
    }

    void calcLightPos0(TVec3f *pPos, const LiveActor *pActor) {
        LightFunction::calcLightWorldPos(pPos, pActor->mActorLightCtrl->getActorLight()->mInfo0);
    }

    void calcLightPos1(TVec3f *pPos, const LiveActor *pActor) {
        LightFunction::calcLightWorldPos(pPos, pActor->mActorLightCtrl->getActorLight()->mInfo1);
    }

    const GXColor* getLightAmbientColor(const LiveActor *pActor) {
        return &pActor->mActorLightCtrl->getActorLight()->mColor;
    }

    ActorLightCtrl* getLightCtrl(const LiveActor *pActor) {
        return pActor->mActorLightCtrl;
    }
};
