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
    static const char* sShadowDefineFileName[] = {"SurfaceCircle",  "SurfaceOval",    "SurfaceBox", "VolumeSphere",    "VolumeOval",
                                                  "VolumeOvalPole", "VolumeCylinder", "VolumeBox",  "VolumeFlatModel", "VolumeLine"};
    template < typename T >
    T* createShadowDrawer(LiveActor* pActor, const JMapInfoIter& rIter, bool surface) {
        ShadowController* pController = ActorShadow::createShadowControlFromCSV(pActor, rIter);
        if (surface) {
            pController->setDropTypeSurface();
        } else {
            pController->setDropTypeNormal();
        }

        T* pDrawer = new T;
        pController->setShadowDrawer(pDrawer);
        return pDrawer;
    }
};  // namespace

namespace ActorShadow {
    u32 getShadowControllerCount(const LiveActor* pActor) {
        return pActor->mShadowControllerList->getControllerCount();
    }

    ShadowController* getShadowController(const LiveActor* pActor, u32 index) {
        return pActor->mShadowControllerList->getController(index);
    }

    ShadowController* getShadowController(const LiveActor* pActor, const char* pName) {
        return pActor->mShadowControllerList->getController(pName);
    }

    bool isExistShadowController(const LiveActor* pActor, const char* pName) {
        if (pActor->mShadowControllerList == nullptr) {
            return false;
        }

        return getShadowController(pActor, pName) != nullptr;
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

    ShadowSurfaceOval* getShadowSurfaceOval(const LiveActor* pActor, const char* pName) {
        return static_cast< ShadowSurfaceOval* >(pActor->mShadowControllerList->getController(pName)->getShadowDrawer());
    }

    ShadowVolumeSphere* getShadowVolumeSphere(const LiveActor* pActor, const char* pName) {
        return static_cast< ShadowVolumeSphere* >(pActor->mShadowControllerList->getController(pName)->getShadowDrawer());
    }

    ShadowVolumeCylinder* getShadowVolumeCylinder(const LiveActor* pActor, const char* pName) {
        return static_cast< ShadowVolumeCylinder* >(pActor->mShadowControllerList->getController(pName)->getShadowDrawer());
    }

    ShadowVolumeBox* getShadowVolumeBox(const LiveActor* pActor, const char* pName) {
        return static_cast< ShadowVolumeBox* >(pActor->mShadowControllerList->getController(pName)->getShadowDrawer());
    }

    ShadowVolumeDrawer* getShadowVolumeDrawer(const LiveActor* pActor, const char* pName) {
        return static_cast< ShadowVolumeDrawer* >(pActor->mShadowControllerList->getController(pName)->getShadowDrawer());
    }

    ShadowController* createShadowControlFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        const char* pName = nullptr;
        rIter.getValue("Name", &pName);
        ShadowController* pController = new ShadowController(pActor, pName);

        const char* pGroupName = "";
        rIter.getValue("GroupName", &pGroupName);
        pController->setGroupName(pGroupName);

        pController->setDropDirPtr(&pActor->mGravity);
        setUpShadowControlFromCSV(pController, pActor, rIter);
        pActor->mShadowControllerList->addController(pController);

        return pController;
    }

    bool getJointNameFromCSV(const char** pJointName, const JMapInfoIter& rIter) {
        *pJointName = "";

        if (rIter.getValue< const char* >("Joint", pJointName)) {
            return *pJointName[0] != '\0';
        }

        return false;
    }

    s32 getShadowTypeFromCSV(const JMapInfoIter& rIter) {
        const char* pType = nullptr;

        if (!rIter.getValue< const char* >("Type", &pType)) {
            return -1;
        }

        for (s32 i = 0; i < ARRAY_SIZE(::sShadowDefineFileName); i++) {
            if (MR::isEqualString(pType, ::sShadowDefineFileName[i])) {
                return i;
            }
        }

        return -1;
    }

    ShadowSurfaceCircle* createShadowSurfaceCircleFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowSurfaceCircle* pShadow = ::createShadowDrawer< ShadowSurfaceCircle >(pActor, rIter, true);

        f32 radius = 100.0f;
        rIter.getValue< f32 >("Radius", &radius);
        pShadow->setRadius(radius);

        return pShadow;
    }

    ShadowSurfaceOval* createShadowSurfaceOvalFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowSurfaceOval* pShadow = ::createShadowDrawer< ShadowSurfaceOval >(pActor, rIter, true);

        TVec3f size;
        size.set< f32 >(100.0f, 100.0f, 100.0f);
        MR::getJMapInfoV3f(rIter, "Size", &size);
        pShadow->setSize(size);

        return pShadow;
    }

    ShadowSurfaceBox* createShadowSurfaceBoxFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowSurfaceBox* pShadow = ::createShadowDrawer< ShadowSurfaceBox >(pActor, rIter, true);

        TVec3f size;
        size.set< f32 >(100.0f, 100.0f, 100.0f);
        MR::getJMapInfoV3f(rIter, "Size", &size);
        pShadow->setSize(size);

        return pShadow;
    }

    ShadowVolumeSphere* createShadowVolumeSphereFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowVolumeSphere* pShadow = ::createShadowDrawer< ShadowVolumeSphere >(pActor, rIter, false);
        setUpShadowVolumeFromCSV(pShadow, rIter);

        f32 radius = 100.0f;
        rIter.getValue< f32 >("Radius", &radius);
        pShadow->setRadius(radius);

        return pShadow;
    }

    ShadowVolumeOval* createShadowVolumeOvalFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowVolumeOval* pShadow = ::createShadowDrawer< ShadowVolumeOval >(pActor, rIter, false);
        setUpShadowVolumeFromCSV(pShadow, rIter);

        TVec3f size;
        size.set< f32 >(100.0f, 100.0f, 100.0f);
        MR::getJMapInfoV3f(rIter, "Size", &size);
        pShadow->setSize(size);

        return pShadow;
    }

    ShadowVolumeOvalPole* createShadowVolumeOvalPoleFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowVolumeOvalPole* pShadow = ::createShadowDrawer< ShadowVolumeOvalPole >(pActor, rIter, false);
        setUpShadowVolumeFromCSV(pShadow, rIter);

        TVec3f size;
        size.set< f32 >(100.0f, 100.0f, 100.0f);
        MR::getJMapInfoV3f(rIter, "Size", &size);
        pShadow->setSize(size);

        return pShadow;
    }

    ShadowVolumeCylinder* createShadowVolumeCylinderFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowVolumeCylinder* pShadow = ::createShadowDrawer< ShadowVolumeCylinder >(pActor, rIter, false);
        setUpShadowVolumeFromCSV(pShadow, rIter);

        f32 radius = 100.0f;
        rIter.getValue< f32 >("Radius", &radius);
        pShadow->setRadius(radius);

        return pShadow;
    }

    ShadowVolumeBox* createShadowVolumeBoxFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowVolumeBox* pShadow = ::createShadowDrawer< ShadowVolumeBox >(pActor, rIter, false);
        setUpShadowVolumeFromCSV(pShadow, rIter);

        TVec3f size;
        size.set< f32 >(100.0f, 100.0f, 100.0f);
        MR::getJMapInfoV3f(rIter, "Size", &size);
        pShadow->setSize(size);

        return pShadow;
    }

    ShadowVolumeFlatModel* createShadowVolumeFlatModelFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowVolumeFlatModel* pShadow = ::createShadowDrawer< ShadowVolumeFlatModel >(pActor, rIter, false);
        setUpShadowVolumeFromCSV(pShadow, rIter);

        const char* pModel = nullptr;
        rIter.getValue< const char* >("Model", &pModel);
        pShadow->initModel(pModel);

        return pShadow;
    }

    ShadowVolumeLine* createShadowVolumeLineFromCSV(LiveActor* pActor, const JMapInfoIter& rIter) {
        ShadowVolumeLine* pShadow = ::createShadowDrawer< ShadowVolumeLine >(pActor, rIter, false);
        setUpShadowVolumeFromCSV(pShadow, rIter);

        const char* pLineStart = nullptr;
        rIter.getValue< const char* >("LineStart", &pLineStart);

        const char* pLineEnd = nullptr;
        rIter.getValue< const char* >("LineEnd", &pLineEnd);

        f32 lineStartRadius = 100.0f;
        rIter.getValue< f32 >("LineStartRadius", &lineStartRadius);

        f32 lineEndRadius = 100.0f;
        rIter.getValue< f32 >("LineEndRadius", &lineEndRadius);

        pShadow->setFromWidth(lineStartRadius);
        pShadow->setToWidth(lineEndRadius);

        pShadow->setFromShadowController(pActor->mShadowControllerList->getController(pLineStart));
        pShadow->setToShadowController(pActor->mShadowControllerList->getController(pLineEnd));

        return pShadow;
    }

    void setUpShadowControlBaseMtxFromCSV(ShadowController* pCtrl, LiveActor* pActor, const JMapInfoIter& rIter) {
        TVec3f dropOffset;

        if (!MR::getJMapInfoV3f(rIter, "DropOffset", &dropOffset)) {
            dropOffset.zero();
        }

        const char* pJointName;

        if (getJointNameFromCSV(&pJointName, rIter)) {
            if (MR::isEqualString(pJointName, "::ACTOR_TRANS")) {
                pCtrl->setDropPosPtr(&pActor->mPosition);
                return;
            }

            if (MR::isEqualString(pJointName, "::BASE_MATRIX")) {
                pCtrl->setDropPosMtxPtr(pActor->getBaseMtx(), dropOffset);
                return;
            }

            if (MR::isEqualString(pJointName, "::FIX_POSITION")) {
                pCtrl->setDropPosFix(pActor->mPosition);
                return;
            }

            if (MR::isEqualString(pJointName, "::OTHER_TRANS")) {
                pCtrl->setDropPosPtr(&pActor->mPosition);
                return;
            }

            if (MR::isEqualString(pJointName, "::OTHER_MATRIX")) {
                pCtrl->setDropPosMtxPtr(pActor->getBaseMtx(), dropOffset);
            } else {
                pCtrl->setDropPosMtxPtr(MR::getJointMtx(pActor, pJointName), dropOffset);
            }
        } else {
            pCtrl->setDropPosPtr(&pActor->mPosition);
        }
    }

    void setUpShadowControlFromCSV(ShadowController* pController, LiveActor* pActor, const JMapInfoIter& rIter) {
        f32 dropLength = 1000.0f;
        rIter.getValue("DropLength", &dropLength);
        pController->setDropLength(dropLength);

        f32 dropOffset = 0.0f;
        rIter.getValue("DropStart", &dropOffset);
        pController->setDropStartOffset(dropOffset);

        setUpShadowControlBaseMtxFromCSV(pController, pActor, rIter);
        setUpShadowControlIsFollowScaleFromCSV(pController, rIter);
        setUpShadowControlIsSyncShowFromCSV(pController, rIter);
        setUpShadowControlCalcCollisionFromCSV(pController, rIter);
        setUpShadowControlCalcGravityFromCSV(pController, rIter);
    }

    void setUpShadowControlIsFollowScaleFromCSV(ShadowController* pController, const JMapInfoIter& rIter) {
        bool followScale = true;
        rIter.getValue("FollowScale", &followScale);

        if (followScale) {
            pController->onFollowHostScale();
        } else {
            pController->offFollowHostScale();
        }
    }

    void setUpShadowControlIsSyncShowFromCSV(ShadowController* pController, const JMapInfoIter& rIter) {
        bool syncShow = true;
        rIter.getValue("SyncShow", &syncShow);

        if (syncShow) {
            pController->onVisibleSyncHost();
        } else {
            pController->offVisibleSyncHost();
        }
    }

    void setUpShadowControlCalcCollisionFromCSV(ShadowController* pCtrl, const JMapInfoIter& rIter) {
        s32 collision = 0;
        rIter.getValue< s32 >("Collision", &collision);

        switch (collision) {
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
            break;
        }
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
