#include "Game/Util/AreaObjUtil.hpp"
#include "Game/AreaObj/AreaForm.hpp"
#include "Game/AreaObj/AreaObj.hpp"
#include "Game/AreaObj/AreaObjContainer.hpp"
#include "Game/AreaObj/RestartCube.hpp"
#include "Game/AreaObj/WaterArea.hpp"
#include "Game/Map/OceanBowl.hpp"
#include "Game/Map/OceanRing.hpp"
#include "Game/Map/OceanSphere.hpp"
#include "Game/Map/WaterAreaHolder.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

void AreaObjUtil_FORCE_MATCH(TVec3f* pVec, const TVec3f& rOther) {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    *pVec += rOther;
}

namespace MR {
    inline AreaObj* getAreaIn(const char* pName, const TVec3f& rPos) {
        return getAreaObjContainer()->getAreaObj(pName, rPos);
    }

    AreaObjMgr* getAreaObjManager(const char* pMgrName) {
        return MR::getAreaObjContainer()->getManager(pMgrName);
    }

    AreaObj* getAreaObj(const char* pAreaName, const TVec3f& rVec) {
        return MR::getAreaObjContainer()->getAreaObj(pAreaName, rVec);
    }

    bool isInAreaObj(const char* pAreaName, const TVec3f& rVec) {
        return MR::getAreaObjContainer()->getAreaObj(pAreaName, rVec);
    }

    s32 getAreaObjArg(const AreaObj* pObj, s32 which) {
        switch (which) {
        case 0:
            return pObj->mObjArg0;
        case 1:
            return pObj->mObjArg1;
        case 2:
            return pObj->mObjArg2;
        case 3:
            return pObj->mObjArg3;
        case 4:
            return pObj->mObjArg4;
        case 5:
            return pObj->mObjArg5;
        case 6:
            return pObj->mObjArg6;
        case 7:
            return pObj->mObjArg7;
        default:
            return -1;
        }
    }

    bool getWaterAreaInfo(WaterInfo* pInfo, const TVec3f& rPos, const TVec3f& rGravity, bool skipConnectedArea) {
        if (pInfo->mOceanBowl != nullptr) {
            return pInfo->mOceanBowl->calcWaterInfo(rPos, rGravity, pInfo);
        }

        if (pInfo->mOceanRing != nullptr) {
            return pInfo->mOceanRing->calcWaterInfo(rPos, rGravity, pInfo);
        }

        if (pInfo->mOceanSphere != nullptr) {
            return pInfo->mOceanSphere->calcWaterInfo(rPos, rGravity, pInfo);
        }

        const WaterArea* pArea = pInfo->mWaterArea;
        f32 depth;

        switch (pArea->mFormType) {
        case AreaForm::Type_Cube1:
        case AreaForm::Type_Cube2: {
            TPos3f worldMtx;
            TRot3f rotationMtx;
            TVec3f up;
            TVec3f top;
            TVec3f localPos;
            pArea->getForm< AreaFormCube >()->calcLocalPos(&localPos, rPos);
            up.set< f32 >(0.0f, 1.0f, 0.0f);
            top = TVec3f(0.0f, pArea->getForm< AreaFormCube >()->mBounding.f.y, 0.0f);
            TVec3f fromTop = localPos - top;
            TVec3f fromBottom(localPos);
            pInfo->mCamWaterDepth = MR::abs(MR::vecKillElement(fromTop, up, &fromTop));
            pInfo->_4 = MR::abs(MR::vecKillElement(fromBottom, up, &fromBottom));
            pInfo->mSurfacePos.set(top + fromTop);
            pArea->getForm< AreaFormCube >()->calcWorldMtx(&worldMtx);
            PSMTXMultVec(worldMtx, &pInfo->mSurfacePos, &pInfo->mSurfacePos);
            TVec3f rotation;
            pArea->getForm< AreaFormCube >()->calcWorldRotate(&rotation);
            MR::makeMtxRotate(rotationMtx, rotation.x, rotation.y, rotation.z);
            rotationMtx.getYDir2(up);
            pInfo->mSurfaceNormal.set(up);

            if (pArea->mObjArg0 > 0) {
                TVec3f streamDir(0.0f, 0.0f, 1.0f);
                MR::calcCubeAxisZ(pArea, &streamDir);
                pInfo->mStreamVec.set(streamDir);
                pInfo->mStreamVec.scale(pArea->mObjArg0);
            }

            break;
        }
        case AreaForm::Type_Sphere: {
            TVec3f center(0.0f, 0.0f, 0.0f);
            pArea->getForm< AreaFormSphere >()->calcPos(&center);
            f32 radius = pArea->getForm< AreaFormSphere >()->mRadius;
            TVec3f radial = rPos - center;
            f32 height = MR::vecKillElement(radial, -rGravity, &radial);
            f32 distanceRate = radial.length() / radius;
            f32 angle = distanceRate;
            angle *= PI;
            f32 theta = angle;
            theta *= 0.5f;
            f32 surfaceHeight = radius * MR::cos(theta);
            pInfo->mCamWaterDepth = surfaceHeight - height;
            pInfo->_4 = surfaceHeight + height;
            TVec3f normal = rPos - center;
            MR::normalizeOrZero(&normal);
            pInfo->mSurfaceNormal.set(normal);
            pInfo->mSurfacePos.set(center + normal * radius);
            break;
        }
        case AreaForm::Type_Bowl: {
            break;
        }
        case AreaForm::Type_Cylinder: {
            TVec3f center(0.0f, 0.0f, 0.0f);
            TVec3f up(0.0f, 0.0f, 1.0f);
            pArea->getForm< AreaFormCylinder >()->calcPos(&center);
            pArea->getForm< AreaFormCylinder >()->calcUpVec(&up);

            if (MR::abs(up.dot(rGravity)) > 0.707f) {
                TVec3f radial;
                f32 height = MR::vecKillElement(rPos - center, up, &radial);
                depth = pArea->getForm< AreaFormCylinder >()->mHeight - height;
                pInfo->_4 = height;
                pInfo->mCamWaterDepth = depth;
                pInfo->mSurfacePos.set(rPos + up * depth);
            } else {
                TVec3f radial;
                MR::vecKillElement(rPos - center, up, &radial);
                f32 distance = radial.length();
                f32 radius = pArea->getForm< AreaFormCylinder >()->mRadius;
                depth = radius - distance;
                pInfo->mCamWaterDepth = depth;
                pInfo->_4 = radius + distance;
                pInfo->mSurfacePos.set(rPos - rGravity * depth);
            }

            pInfo->mSurfaceNormal.set(-rGravity);

            if (pArea->mObjArg0 > 0) {
                TVec3f streamDir(0.0f, 0.0f, 0.0f);
                pArea->getForm< AreaFormCylinder >()->calcUpVec(&streamDir);
                pInfo->mStreamVec.set(streamDir);
                pInfo->mStreamVec.scale(pArea->mObjArg0);
            }

            break;
        }
        }

        if (!skipConnectedArea) {
            TVec3f checkPos(pInfo->mSurfacePos - rGravity * 5.0f);
            WaterInfo nextInfo;
            MR::getWaterAreaObj(&nextInfo, checkPos);

            if (nextInfo.isInWater()) {
                MR::getWaterAreaInfo(&nextInfo, checkPos, rGravity, true);
                f32 nextDepth = nextInfo.mCamWaterDepth;
                pInfo->mCamWaterDepth = pInfo->mCamWaterDepth + nextDepth;
                pInfo->mSurfacePos -= rGravity * nextInfo.mCamWaterDepth;
            }
        }

        return pArea->isInVolume(rPos);
    }

    bool calcWhirlPoolAccelInfo(const TVec3f& rVec, TVec3f* pVec) {
        return WaterAreaFunction::tryInWhirlPoolAccelerator(rVec, pVec);
    }

    void calcCubePos(const AreaObj* pArea, TVec3f* pPos) {
        pArea->getForm< AreaFormCube >()->calcWorldPos(pPos);
    }

    void calcCubeRotate(const AreaObj* pArea, TVec3f* pPos) {
        pArea->getForm< AreaFormCube >()->calcWorldRotate(pPos);
    }

    void calcCubeAxisZ(const AreaObj* pArea, TVec3f* pPos) {
        TVec3f rotate;
        pArea->getForm< AreaFormCube >()->calcWorldRotate(&rotate);
        TRot3f rotation;
        MR::makeMtxRotate(rotation, rotate.x, rotate.y, rotate.z);
        rotation.getZDir2(*pPos);
    }

    void calcCubeWorldBox(TDirBox3f* pBox, const AreaObj* pArea) {
        pArea->getForm< AreaFormCube >()->calcWorldBox(pBox);
    }

    TBox3f* getCubeLocalBox(const AreaObj* pArea) {
        return &pArea->getForm< AreaFormCube >()->mBounding;
    }

    void calcCubeLocalPos(TVec3f* pVec, const AreaObj* pArea, const TVec3f& rVec) {
        pArea->getForm< AreaFormCube >()->calcLocalPos(pVec, rVec);
    }

    void calcSpherePos(TVec3f* pVec, const AreaObj* pArea) {
        pArea->getForm< AreaFormSphere >()->calcPos(pVec);
    }

    f32 getSphereRadius(const AreaObj* pArea) {
        return pArea->getForm< AreaFormSphere >()->mRadius;
    }

    void calcCylinderPos(TVec3f* pVec, const AreaObj* pArea) {
        pArea->getForm< AreaFormCylinder >()->calcPos(pVec);
    }

    void calcCylinderCenterPos(TVec3f* pVec, const AreaObj* pArea) {
        pArea->getForm< AreaFormCylinder >()->calcCenterPos(pVec);
    }

    void calcCylinderUpVec(TVec3f* pVec, const AreaObj* pArea) {
        pArea->getForm< AreaFormCylinder >()->calcUpVec(pVec);
    }

    f32 getCylinderRadius(const AreaObj* pArea) {
        return pArea->getForm< AreaFormCylinder >()->mRadius;
    }

    void tryToUpdatePlayerRestartIdInfo(const TVec3f& rVec) {
        RestartCube* pCube = MR::getAreaObj< RestartCube >("RestartCube", rVec);

        if (pCube != nullptr) {
            pCube->updatePlayerRestartIdInfo();
        }
    }

    bool getWaterAreaObj(WaterInfo* pInfo, const TVec3f& rPos) {
        pInfo->clear();
        WaterArea* pArea = getAreaObj< WaterArea >("Water", rPos);

        if (pArea != nullptr) {
            pInfo->mWaterArea = pArea;
            return true;
        }

        return WaterAreaFunction::tryInOceanArea(rPos, pInfo);
    }

    bool calcAreaMoveVelocity(TVec3f* pVelocity, const TVec3f& rPos) {
        AreaObj* pArea = MR::getAreaObj("AreaMoveSphere", rPos);

        if (pArea == nullptr) {
            pVelocity->zero();
            return false;
        }

        TVec3f center;
        pArea->getForm< AreaFormSphere >()->calcPos(&center);
        TVec3f up;
        pArea->getForm< AreaFormSphere >()->calcUpVec(&up);
        TVec3f radial = center - rPos;
        MR::normalizeOrZero(&radial);
        MR::vecKillElement(up, radial, &up);
        MR::normalizeOrZero(&up);
        s32 speed = MR::getAreaObjArg(pArea, 0);

        if (speed == -1) {
            speed = 10;
        }

        pVelocity->set(up * speed);
        return true;
    }

    AreaObj* getCurrentAstroOverlookAreaObj() {
        return getAreaIn("AstroOverlookArea", *MR::getPlayerPos());
    }
};  // namespace MR
