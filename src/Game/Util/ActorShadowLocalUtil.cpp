#include "Game/Util.hpp"
#include "Game/Util/ActorShadowLocalUtil.hpp"

namespace ActorShadow {
    u32 getShadowControllerCount(const LiveActor* pActor) {
        return pActor->mShadowControllerList->getControllerCount();
    }

    ShadowController* getShadowController(const LiveActor* pActor, u32 a2) {
        return pActor->mShadowControllerList->getController(a2);
    }

    ShadowController* getShadowController(const LiveActor* pActor, const char* pName) {
        return pActor->mShadowControllerList->getController(pName);
    }

    ShadowController* createShadowControllerVolumeParam(LiveActor* pActor, const char* pName) {
        ShadowController* pController = new ShadowController(pActor, pName);
        pController->setDropPosPtr(&pActor->mPosition);
        pController->setDropDirPtr(&pActor->mGravity);
        pController->setDropLength(1000.0f);
        pController->setDropTypeNormal();
        pActor->mShadowControllerList->addController(pController);
        return pController;
    }

    ShadowController* createShadowControllerSuefaceParam(LiveActor* pActor, const char* pName) {
        ShadowController* pController = new ShadowController(pActor, pName);
        pController->setDropPosPtr(&pActor->mPosition);
        pController->setDropDirPtr(&pActor->mGravity);
        pController->setDropLength(1000.0f);
        pController->setDropTypeSurface();
        pActor->mShadowControllerList->addController(pController);
        return pController;
    }

    ShadowDrawer* getShadowSurfaceOval(const LiveActor* pActor, const char* pName) {
        return pActor->mShadowControllerList->getController(pName)->getShadowDrawer();
    }

    ShadowDrawer* getShadowVolumeSphere(const LiveActor* pActor, const char* pName) {
        return pActor->mShadowControllerList->getController(pName)->getShadowDrawer();
    }

    ShadowDrawer* getShadowVolumeCylinder(const LiveActor* pActor, const char* pName) {
        return pActor->mShadowControllerList->getController(pName)->getShadowDrawer();
    }

    ShadowDrawer* getShadowVolumeBox(const LiveActor* pActor, const char* pName) {
        return pActor->mShadowControllerList->getController(pName)->getShadowDrawer();
    }

    ShadowDrawer* getShadowVolumeDrawer(const LiveActor* pActor, const char* pName) {
        return pActor->mShadowControllerList->getController(pName)->getShadowDrawer();
    }

    // Fails as JMapInfo::getValueFast lacks an implementation that returns a bool pointer
    /* void setUpShadowControlIsFollowScaleFromCSV(ShadowController *pController, const JMapInfoIter &rIter) {
        bool stack_8(true);
        rIter.getValue("FollowScale", &stack_8);
        if (stack_8) {
            pController->onFollowHostScale();
        }
        else {
            pController->offFollowHostScale();
        }
    } 

    void setUpShadowControlIsSyncShowFromCSV(ShadowController *pController, const JMapInfoIter &rIter) {
        bool stack_8(true);
        rIter.getValue("SyncShow", &stack_8);
        if (stack_8) {
            pController->onVisibleSyncHost();
        }
        else {
            pController->offVisibleSyncHost();
        }
    } */

    void setUpShadowControlFromCSV(ShadowController* pController, LiveActor* pActor, const JMapInfoIter& rIter) {
        float dropLength(1000.0f);
        rIter.getValue("DropLength", &dropLength);
        pController->setDropLength(dropLength);
        float dropOffset(0.0f);
        rIter.getValue("DropStart", &dropOffset);
        pController->setDropStartOffset(dropOffset);
        setUpShadowControlBaseMtxFromCSV(pController, pActor, rIter);
        setUpShadowControlIsFollowScaleFromCSV(pController, rIter);
        setUpShadowControlIsSyncShowFromCSV(pController, rIter);
        setUpShadowControlCalcCollisionFromCSV(pController, rIter);
        setUpShadowControlCalcGravityFromCSV(pController, rIter);
    }

    /* ShadowController* createShadowControlFromCSV(LiveActor *pActor, const JMapInfoIter &rIter) {
        char* stack_c(nullptr);
        rIter.getValue("Name", &stack_c);
        ShadowController* pController = new ShadowController(pActor, stack_c);
        char* stack_8(nullptr); // Actual value loaded is lbl_806B2600
        rIter.getValue("GroupName", &stack_8);
        pController->setGroupName(stack_8);
        pController->setDropDirPtr(&pActor->mGravity);
        setUpShadowControlFromCSV(pController, pActor, rIter);
        pActor->mShadowControllerList->addController(pController);
        return pController;
    } */

    // Minor mismatch: register swap
    /* ShadowSurfaceCircle* createShadowSurfaceCircleFromCSV(LiveActor *pActor, const JMapInfoIter &rIter) {
        ShadowController* pController = createShadowControlFromCSV(pActor, rIter);
        pController->setDropTypeSurface();
        ShadowSurfaceCircle* pSurfaceCircle = new ShadowSurfaceCircle();
        pController->setShadowDrawer(pSurfaceCircle);
        float radius(100.0f);
        rIter.getValue("Radius", &radius);
        pSurfaceCircle->setRadius(radius);
        return pSurfaceCircle;
    } */
}; // namespace ActorShadow
