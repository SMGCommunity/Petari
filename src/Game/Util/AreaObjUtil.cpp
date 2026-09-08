#include "Game/Util/AreaObjUtil.hpp"
#include "Game/AreaObj/AreaForm.hpp"
#include "Game/AreaObj/AreaObj.hpp"
#include "Game/AreaObj/AreaObjContainer.hpp"
#include "Game/AreaObj/RestartCube.hpp"
#include "Game/AreaObj/WaterArea.hpp"
#include "Game//Map/WaterAreaHolder.hpp"
#include "Game/Util.hpp"

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

    /* AreaInfo* getWaterAreaInfo(WaterInfo* pInfo, const TVec3f& rVec, const TVec3f& rVec2, bool b) {

    } */

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

    /* bool calcAreaMoveVelocity(TVec3f * pVec, const TVec3f & rVec) {
        AreaObj* pArea = MR::getAreaObj<RestartCube>("AreaMoveSphere", rVec);
        if (pArea == nullptr) {
            pVec->zero();
            return false;
        }
        TVec3f pos;
        pArea->getForm<AreaFormSphere>()->calcPos(&pos);
        TVec3f upVec;
        pArea->getForm<AreaFormSphere>()->calcUpVec(&upVec);
        TVec3f vec(pos);
        *pVec -= vec;
        MR::normalizeOrZero(&vec);
        MR::vecKillElement(upVec, vec, &upVec);
        if (MR::getAreaObjArg(pArea, 0) == -1) {

        }
    } */

    AreaObj* getCurrentAstroOverlookAreaObj() {
        return getAreaIn("AstroOverlookArea", *MR::getPlayerPos());
    }
};  // namespace MR
