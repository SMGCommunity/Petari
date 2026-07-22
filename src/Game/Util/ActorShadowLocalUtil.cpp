#include "Game/Util/ActorShadowLocalUtil.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ShadowController.hpp"
#include "Game/LiveActor/ShadowSurfaceBox.hpp"
#include "Game/LiveActor/ShadowSurfaceCircle.hpp"
#include "Game/LiveActor/ShadowSurfaceOval.hpp"
#include "Game/LiveActor/ShadowVolumeBox.hpp"
#include "Game/LiveActor/ShadowVolumeCylinder.hpp"
#include "Game/LiveActor/ShadowVolumeDrawer.hpp"
#include "Game/LiveActor/ShadowVolumeFlatModel.hpp"
#include "Game/LiveActor/ShadowVolumeLine.hpp"
#include "Game/LiveActor/ShadowVolumeOval.hpp"
#include "Game/LiveActor/ShadowVolumeOvalPole.hpp"
#include "Game/LiveActor/ShadowVolumeSphere.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    static const char* sShadowDefineFileName[10] = {"SurfaceCircle",  "SurfaceOval",    "SurfaceBox", "VolumeSphere",    "VolumeOval",
                                                    "VolumeOvalPole", "VolumeCylinder", "VolumeBox",  "VolumeFlatModel", "VolumeLine"};
};

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

    bool isExistShadowController(const LiveActor* pActor, const char* pName) {
        if (pActor->mShadowControllerList == nullptr) {
            return false;
        }

        return pActor->mShadowControllerList->getController(pName) != nullptr;
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

    bool getJointNameFromCSV(const char** pJointName, const JMapInfoIter& rIter) {
        *pJointName = "";
        if (rIter.getValue< const char* >("Joint", pJointName)) {
            return *pJointName[0] != 0;
        } else {
            return false;
        }
    }

    s32 getShadowTypeFromCSV(const JMapInfoIter& rIter) {
        const char* type = 0;
        if (!rIter.getValue< const char* >("Type", &type)) {
            return -1;
        }

        for (s32 i = 0; i < ARRAY_SIZE(sShadowDefineFileName); i++) {
            if (MR::isEqualString(type, sShadowDefineFileName[i])) {
                return i;
            }
        }

        return -1;
    }

    void setUpShadowControlBaseMtxFromCSV(ShadowController* pCtrl, LiveActor* pActor, const JMapInfoIter& rIter) {
        TVec3f dropOffs;
        if (!MR::getJMapInfoV3f(rIter, "DropOffset", &dropOffs)) {
            dropOffs.z = 0.0f;
            dropOffs.y = 0.0f;
            dropOffs.x = 0.0f;
        }

        const char* jointName;
        if (getJointNameFromCSV(&jointName, rIter)) {
            if (MR::isEqualString(jointName, "::ACTOR_TRANS")) {
                pCtrl->setDropPosPtr(&pActor->mPosition);
                return;
            }

            if (MR::isEqualString(jointName, "::BASE_MATRIX")) {
                pCtrl->setDropPosMtxPtr(pActor->getBaseMtx(), dropOffs);
                return;
            }

            if (MR::isEqualString(jointName, "::FIX_POSITION")) {
                pCtrl->setDropPosFix(pActor->mPosition);
                return;
            }
            if (MR::isEqualString(jointName, "::OTHER_TRANS")) {
                pCtrl->setDropPosPtr(&pActor->mPosition);
                return;
            }

            if (MR::isEqualString(jointName, "::OTHER_MATRIX")) {
                pCtrl->setDropPosMtxPtr(pActor->getBaseMtx(), dropOffs);
            } else {
                pCtrl->setDropPosMtxPtr(MR::getJointMtx(pActor, jointName), dropOffs);
            }
        } else {
            pCtrl->setDropPosPtr(&pActor->mPosition);
            return;
        }
    }

    void setUpShadowControlIsFollowScaleFromCSV(ShadowController* pController, const JMapInfoIter& rIter) {
        bool stack_8(true);
        rIter.getValue("FollowScale", &stack_8);
        if (stack_8) {
            pController->onFollowHostScale();
        } else {
            pController->offFollowHostScale();
        }
    }

    void setUpShadowControlIsSyncShowFromCSV(ShadowController* pController, const JMapInfoIter& rIter) {
        bool stack_8(true);
        rIter.getValue("SyncShow", &stack_8);
        if (stack_8) {
            pController->onVisibleSyncHost();
        } else {
            pController->offVisibleSyncHost();
        }
    }

    void setUpShadowControlCalcCollisionFromCSV(ShadowController* pCtrl, const JMapInfoIter& rIter) {
        s32 col = 0;
        rIter.getValue< s32 >("Collision", &col);

        switch (col) {
        case 0:
            pCtrl->offCalcCollision();
            break;
        case 1:
            pCtrl->onCalcCollision();
            break;
        case 2:
            pCtrl->onCalcCollisionOneTime();
            break;
        }
    }

    void setUpShadowControlCalcGravityFromCSV(ShadowController* pCtrl, const JMapInfoIter& rIter) {
        s32 gravity = 0;
        rIter.getValue< s32 >("Gravity", &gravity);

        switch (gravity) {
        case 0:
            pCtrl->offCalcDropGravity();
            break;
        case 1:
            pCtrl->onCalcDropGravity();
            break;
        case 2:
            pCtrl->onCalcDropGravityOneTime();
            break;
        case 3:
            pCtrl->offCalcDropPrivateGravity();
            break;
        case 4:
            pCtrl->onCalcDropPrivateGravity();
            break;
        case 5:
            pCtrl->onCalcDropPrivateGravityOneTime();
        }
    }

    void setUpShadowControlFromCSV(ShadowController* pController, LiveActor* pActor, const JMapInfoIter& rIter) {
        f32 dropLength(1000.0f);
        rIter.getValue("DropLength", &dropLength);
        pController->setDropLength(dropLength);
        f32 dropOffset(0.0f);
        rIter.getValue("DropStart", &dropOffset);
        pController->setDropStartOffset(dropOffset);
        setUpShadowControlBaseMtxFromCSV(pController, pActor, rIter);
        setUpShadowControlIsFollowScaleFromCSV(pController, rIter);
        setUpShadowControlIsSyncShowFromCSV(pController, rIter);
        setUpShadowControlCalcCollisionFromCSV(pController, rIter);
        setUpShadowControlCalcGravityFromCSV(pController, rIter);
    }

    ShadowController* createShadowControlFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        const char* name = "";
        rIter.getValue("Name", &name);
        ShadowController* pController = new ShadowController(pActor, name);
        const char* groupName = "";
        rIter.getValue("GroupName", &groupName);
        pController->setGroupName(groupName);
        pController->setDropDirPtr(&pActor->mGravity);
        setUpShadowControlFromCSV(pController, pActor, rIter);
        pActor->mShadowControllerList->addController(pController);
        return pController;
    }

    void setUpShadowVolumeFromCSV(ShadowVolumeDrawer* pDrawer, const JMapInfoIter& rIter) {
        f32 volumeStart = 100.0f;
        f32 volumeEnd = 100.0f;
        s32 volumeCut = 0;
        rIter.getValue< f32 >("VolumeStart", &volumeStart);
        rIter.getValue< f32 >("VolumeEnd", &volumeEnd);
        rIter.getValue< s32 >("VolumeCut", &volumeCut);
        pDrawer->setStartDrawShepeOffset(volumeStart);
        pDrawer->setEndDrawShepeOffset(volumeEnd);

        if (volumeCut != 0) {
            pDrawer->onCutDropShadow();
        } else {
            pDrawer->offCutDropShadow();
        }
    }

    ShadowSurfaceCircle* createShadowSurfaceCircleFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowController* ctrl = createShadowControlFromCSV(pActor, rIter);
        ctrl->setDropTypeSurface();

        ShadowSurfaceCircle* shadow = new ShadowSurfaceCircle();
        ctrl->setShadowDrawer(shadow);
        f32 radius = 100.0f;
        rIter.getValue< f32 >("Radius", &radius);
        shadow->setRadius(radius);
        return shadow;
    }

    ShadowSurfaceOval* createShadowSurfaceOvalFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowController* ctrl = createShadowControlFromCSV(pActor, rIter);
        ctrl->setDropTypeSurface();

        ShadowSurfaceOval* shadow = new ShadowSurfaceOval();
        ctrl->setShadowDrawer(shadow);

        TVec3f size;
        size.set< f32 >(100.0f, 100.0f, 100.0f);
        MR::getJMapInfoV3f(rIter, "Size", &size);
        shadow->setSize(size);
        return shadow;
    }

    ShadowSurfaceBox* createShadowSurfaceBoxFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowController* ctrl = createShadowControlFromCSV(pActor, rIter);
        ctrl->setDropTypeSurface();

        ShadowSurfaceBox* shadow = new ShadowSurfaceBox();
        ctrl->setShadowDrawer(shadow);

        TVec3f size;
        size.set< f32 >(100.0f, 100.0f, 100.0f);
        MR::getJMapInfoV3f(rIter, "Size", &size);
        shadow->setSize(size);
        return shadow;
    }

    ShadowVolumeSphere* createShadowVolumeSphereFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowController* ctrl = createShadowControlFromCSV(pActor, rIter);
        ctrl->setDropTypeNormal();

        ShadowVolumeSphere* shadow = new ShadowVolumeSphere();
        ctrl->setShadowDrawer(shadow);
        setUpShadowVolumeFromCSV(shadow, rIter);

        f32 radius = 100.0f;
        rIter.getValue< f32 >("Radius", &radius);
        shadow->setRadius(radius);
        return shadow;
    }

    ShadowVolumeOval* createShadowVolumeOvalFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowController* ctrl = createShadowControlFromCSV(pActor, rIter);
        ctrl->setDropTypeNormal();

        ShadowVolumeOval* shadow = new ShadowVolumeOval();
        ctrl->setShadowDrawer(shadow);
        setUpShadowVolumeFromCSV(shadow, rIter);

        TVec3f size;
        size.set< f32 >(100.0f, 100.0f, 100.0f);
        MR::getJMapInfoV3f(rIter, "Size", &size);
        shadow->setSize(size);
        return shadow;
    }

    ShadowVolumeOvalPole* createShadowVolumeOvalPoleFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowController* ctrl = createShadowControlFromCSV(pActor, rIter);
        ctrl->setDropTypeNormal();

        ShadowVolumeOvalPole* shadow = new ShadowVolumeOvalPole();
        ctrl->setShadowDrawer(shadow);
        setUpShadowVolumeFromCSV(shadow, rIter);

        TVec3f size;
        size.set< f32 >(100.0f, 100.0f, 100.0f);
        MR::getJMapInfoV3f(rIter, "Size", &size);
        shadow->setSize(size);
        return shadow;
    }

    ShadowVolumeCylinder* createShadowVolumeCylinderFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowController* ctrl = createShadowControlFromCSV(pActor, rIter);
        ctrl->setDropTypeNormal();

        ShadowVolumeCylinder* shadow = new ShadowVolumeCylinder();
        ctrl->setShadowDrawer(shadow);
        setUpShadowVolumeFromCSV(shadow, rIter);

        f32 radius = 100.0f;
        rIter.getValue< f32 >("Radius", &radius);
        shadow->setRadius(radius);
        return shadow;
    }

    ShadowVolumeBox* createShadowVolumeBoxFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowController* ctrl = createShadowControlFromCSV(pActor, rIter);
        ctrl->setDropTypeNormal();

        ShadowVolumeBox* shadow = new ShadowVolumeBox();
        ctrl->setShadowDrawer(shadow);
        setUpShadowVolumeFromCSV(shadow, rIter);

        TVec3f size;
        size.set< f32 >(100.0f, 100.0f, 100.0f);
        MR::getJMapInfoV3f(rIter, "Size", &size);
        shadow->setSize(size);
        return shadow;
    }

    ShadowVolumeFlatModel* createShadowVolumeFlatModelFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowController* ctrl = createShadowControlFromCSV(pActor, rIter);
        ctrl->setDropTypeNormal();

        ShadowVolumeFlatModel* shadow = new ShadowVolumeFlatModel();
        ctrl->setShadowDrawer(shadow);
        setUpShadowVolumeFromCSV(shadow, rIter);

        const char* model = nullptr;
        rIter.getValue< const char* >("Model", &model);
        shadow->initModel(model);

        return shadow;
    }

    ShadowVolumeLine* createShadowVolumeLineFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowController* ctrl = createShadowControlFromCSV(pActor, rIter);
        ctrl->setDropTypeNormal();

        ShadowVolumeLine* shadow = new ShadowVolumeLine();
        ctrl->setShadowDrawer(shadow);
        setUpShadowVolumeFromCSV(shadow, rIter);

        const char* lineStart = nullptr;
        rIter.getValue< const char* >("LineStart", &lineStart);
        const char* lineEnd = nullptr;
        rIter.getValue< const char* >("LineEnd", &lineEnd);
        f32 lineStartRadius = 100.0f;
        rIter.getValue< f32 >("LineStartRadius", &lineStartRadius);
        f32 lineEndRadius = 100.0f;
        rIter.getValue< f32 >("LineEndRadius", &lineEndRadius);

        shadow->setFromWidth(lineStartRadius);
        shadow->setToWidth(lineEndRadius);

        shadow->setFromShadowController(pActor->mShadowControllerList->getController(lineStart));
        shadow->setToShadowController(pActor->mShadowControllerList->getController(lineEnd));
        return shadow;
    }

    void addShadowFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        s32 type = getShadowTypeFromCSV(rIter);

        switch (type) {
        case 0:
            createShadowSurfaceCircleFromCSV(pActor, rIter);
            break;
        case 1:
            createShadowSurfaceOvalFromCSV(pActor, rIter);
            break;
        case 2:
            createShadowSurfaceBoxFromCSV(pActor, rIter);
            break;
        case 3:
            createShadowVolumeSphereFromCSV(pActor, rIter);
            break;
        case 4:
            createShadowVolumeOvalFromCSV(pActor, rIter);
            break;
        case 5:
            createShadowVolumeOvalPoleFromCSV(pActor, rIter);
            break;
        case 6:
            createShadowVolumeCylinderFromCSV(pActor, rIter);
            break;
        case 7:
            createShadowVolumeBoxFromCSV(pActor, rIter);
            break;
        case 8:
            createShadowVolumeFlatModelFromCSV(pActor, rIter);
            break;
        case 9:
            createShadowVolumeLineFromCSV(pActor, rIter);
            break;
        }
    }
};  // namespace ActorShadow
