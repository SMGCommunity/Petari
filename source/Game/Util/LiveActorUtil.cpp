#include "Game/Util.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/ActorLightCtrl.h"
#include "Game/LiveActor/ClippingDirector.h"
#include "Game/Map/LightFunction.h"

namespace MR {
    void initDefaultPos(LiveActor *pActor, const JMapInfoIter &rIter) {
        if (!rIter.isValid()) {
            return;
        }

        MR::getJMapInfoTrans(rIter, &pActor->mPosition);
        MR::getJMapInfoRotate(rIter, &pActor->mRotation);
        MR::getJMapInfoScale(rIter, &pActor->mScale);

        f32 rot_x = fmod(360.0f + (pActor->mRotation.x) - 0.0f, 360.0f);
        f32 rot_y = fmod(360.0f + (pActor->mRotation.y) -  0.0f, 360.0f);
        pActor->mRotation.x = 0.0f + rot_x;
    }

    void initDefaultPosNoRepeat(LiveActor *pActor, const JMapInfoIter &rIter) {
        MR::getJMapInfoTrans(rIter, &pActor->mPosition);
        MR::getJMapInfoRotate(rIter, &pActor->mRotation);
        MR::getJMapInfoScale(rIter, &pActor->mScale);
    }

    bool isValidMovement(const LiveActor *pActor) {
        if (pActor->mFlags.mIsDead || pActor->mFlags.mIsClipped) {
            return false;
        }

        return true;
    }

    bool isValidCalcAnim(const LiveActor *pActor) {
        if (pActor->mFlags.mIsDead || pActor->mFlags.mIsClipped || pActor->mFlags.mIsNoCalcAnim) {
            return false;
        }

        return true;
    }

    bool isValidCalcViewAndEntry(const LiveActor *pActor) {
        if (pActor->mFlags.mIsDead || pActor->mFlags.mIsClipped) {
            return false;
        }

        return true;
    }

    bool isValidDraw(const LiveActor *pActor) {
        if (pActor->mFlags.mIsDead || pActor->mFlags.mIsClipped || pActor->mFlags.mIsHiddenModel) {
            return false;
        }

        return true;
    }

    void calcAnimDirect(LiveActor *pActor) {
        bool isNoCalcAnim = pActor->mFlags.mIsNoCalcAnim;
        pActor->mFlags.mIsNoCalcAnim = false;
        pActor->calcAnim();
        if (isNoCalcAnim) {
            pActor->mFlags.mIsNoCalcAnim = true;
        }
    }

    void setClippingTypeSphere(LiveActor *pActor, f32 rad) {
        MR::getClippingDirector()->mActorHolder->setTypeToSphere(pActor, rad, 0);
    }

    void setClippingTypeSphere(LiveActor *pActor, f32 rad, const TVec3f *a3) {
        MR::getClippingDirector()->mActorHolder->setTypeToSphere(pActor, rad, a3);
    }

    void setClippingTypeSphereContainsModelBoundingBox(LiveActor *pActor, f32 a2) {
        f32 radius = 0.0f;
        MR::calcModelBoundingRadius(&radius, pActor);
        f32 rad = radius * a2;
        MR::getClippingDirector()->mActorHolder->setTypeToSphere(pActor, rad, 0);
    }

    void setClippingFar50m(LiveActor *pActor) {
        MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 7);
    }

    void setClippingFar100m(LiveActor *pActor) {
        MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 6);
    }

    void setClippingFar200m(LiveActor *pActor) {
        MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 5);
    }

    void setClippingFar300m(LiveActor *pActor) {
        MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 4);
    }

    void setClippingFarMax(LiveActor *pActor) {
        MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 0);
    }

    void setClippingFar(LiveActor *pActor, f32 clipping) {
        s32 clip = clipping;
        switch (clip) {
            case 0x32:
                MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 7);
                break;
            case 0x64:
                MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 6);
                break;
            case 0xC8:
                MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 5);
                break;
            case 0x12C:
                MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 4);
                break;
            case 0x190:
                MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 3);
                break;
            case 0x1F4:
                MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 2);
                break;
            case 0x258:
                MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 1);
                break;
            case -1:
                MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 0);
                break;
        }
    }

    void setGroupClipping(LiveActor *pActor, const JMapInfoIter &rIter, int a3) {
        MR::getClippingDirector()->joinToGroupClipping(pActor, rIter, a3);
    }

    void validateClipping(LiveActor *pActor) {
        if (pActor->mFlags.mIsInvalidClipping) {
            MR::getClippingDirector()->mActorHolder->validateClipping(pActor);
        }
    }

    void invalidateClipping(LiveActor *pActor) {
        if (pActor->mFlags.mIsInvalidClipping) {
            if (pActor->mFlags.mIsClipped) {
                pActor->endClipped();
            }
        }
        else {
            MR::getClippingDirector()->mActorHolder->invalidateClipping(pActor);
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

    bool isStep(const LiveActor *pActor, s32 step) {
        return pActor->getNerveStep() == step;
    }

    /*
    bool isFirstStep(const LiveActor *pActor) {
        return MR::isStep(pActor, 0);
    }
    */

   bool isLessStep(const LiveActor *pActor, s32 step) {
        return pActor->getNerveStep() < step;
   }

   bool isLessEqualStep(const LiveActor *pActor, s32 step) {
        return pActor->getNerveStep() <= step;
   }

   bool isGreaterStep(const LiveActor *pActor, s32 step) {
        return pActor->getNerveStep() > step;
   }

   bool isGreaterEqualStep(const LiveActor *pActor, s32 step) {
        return pActor->getNerveStep() >= step;
   }

   bool isIntervalStep(const LiveActor *pActor, s32 step) {
        s32 curStep = pActor->getNerveStep();
        s32 ratio = curStep / step;
        s32 mult = ratio * step;
        return mult == curStep;
    }

    
};
