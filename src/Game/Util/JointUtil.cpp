#include "Game/Util/JointUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "JSystem/J3DGraphBase/J3DMaterial.hpp"
#include <JSystem/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3DGraphAnimator/J3DModelData.hpp>
#include <JSystem/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3DGraphAnimator/J3DMtxBuffer.hpp>
#include <JSystem/J3DGraphBase/J3DShape.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JGeometry.hpp>

namespace MR {
    J3DJoint* getJoint(J3DModel* pModel, const char* pName) {
        return pModel->mModelData->mJointTree.mJointNode[(u16)pModel->mModelData->mJointTree.mJointName->getIndex(pName)];
    }

    J3DJoint* getJoint(const LiveActor* pActor, const char* pName) {
        return MR::getJoint(MR::getJ3DModel(pActor), pName);
    }

    J3DJoint* getJoint(const LiveActor* pActor, u16 idx) {
        return MR::getJ3DModelData(pActor)->mJointTree.mJointNode[idx];
    }

    MtxPtr getJointMtx(J3DModel* pModel, const char* pName) {
        s32 idx = pModel->mModelData->mJointTree.mJointName->getIndex(pName);
        return pModel->mMtxBuffer->mpAnmMtx[idx];
    }

    MtxPtr getJointMtx(const LiveActor* pActor, const char* pName) {
        s32 idx = MR::getJointIndex(pActor, pName);
        return MR::getJ3DModel(pActor)->mMtxBuffer->mpAnmMtx[idx];
    }

    MtxPtr getJointMtx(const LiveActor* pActor, int idx) {
        return MR::getJ3DModel(pActor)->mMtxBuffer->mpAnmMtx[idx];
    }

    s32 getJointIndex(const LiveActor* pActor, const char* pName) {
        return MR::getJ3DModelData(pActor)->mJointTree.mJointName->getIndex(pName);
    }

    const char* getJointName(const LiveActor* pActor, int idx) {
        return MR::getJ3DModelData(pActor)->mJointTree.mJointName->getName(idx);
    }

    s32 getJointNum(const LiveActor* pActor) {
        return MR::getJ3DModelData(pActor)->mJointTree.mJointNum;
    }

    bool isExistJoint(const LiveActor* pActor, const char* pName) {
        return MR::getJ3DModelData(pActor)->mJointTree.mJointName->getIndex(pName) != -1;
    }

    void copyJointPos(J3DModel* pModel, const char* pName, TVec3f* pPos) {
        MtxPtr jointMtx = MR::getJointMtx(pModel, pName);
        pPos->set< f32 >(jointMtx[0][3], jointMtx[1][3], jointMtx[2][3]);
    }

    void copyJointPos(const LiveActor* pActor, const char* pName, TVec3f* pPos) {
        copyJointPos(MR::getJ3DModel(pActor), pName, pPos);
    }

    void copyJointPos(J3DModel* pModel, int idx, TVec3f* pPos) {
        MtxPtr jointMtx = pModel->mMtxBuffer->mpAnmMtx[idx];
        pPos->set< f32 >(jointMtx[0][3], jointMtx[1][3], jointMtx[2][3]);
    }

    void copyJointPos(const LiveActor* pActor, int idx, TVec3f* pPos) {
        copyJointPos(MR::getJ3DModel(pActor), idx, pPos);
    }

    void copyJointScale(const LiveActor* pActor, const char* pName, TVec3f* pScale) {
        TMtx34f mtx;
        mtx.set(MR::getJointMtx(pActor, pName));
        pScale->x = JGeometry::TUtil< f32 >::sqrt(mtx(2, 0) * mtx(2, 0) + (mtx(0, 0) * mtx(0, 0) + mtx(1, 0) * mtx(1, 0)));
        pScale->y = JGeometry::TUtil< f32 >::sqrt(mtx(2, 1) * mtx(2, 1) + (mtx(0, 1) * mtx(0, 1) + mtx(1, 1) * mtx(1, 1)));
        pScale->z = JGeometry::TUtil< f32 >::sqrt(mtx(2, 2) * mtx(2, 2) + (mtx(0, 2) * mtx(0, 2) + mtx(1, 2) * mtx(1, 2)));
    }

    void hideJoint(J3DJoint* pJoint) {
        for (J3DMaterial* mat = pJoint->mMesh; mat != nullptr; mat = mat->_4) {
            mat->mShape->mFlags |= 1;
        }
    }

    void hideJoint(J3DModel* pModel, const char* pName) {
        MR::hideJoint(MR::getJoint(pModel, pName));
    }

    void hideJoint(const LiveActor* pActor, const char* pName) {
        hideJoint(MR::getJ3DModel(pActor), pName);
    }

    void hideJointAndChildren(J3DJoint* pJoint) {
        MR::hideJoint(pJoint);
        J3DJoint *j, *i;
        for (i = pJoint->mChild; i; i = i->mChild) {
            for (j = i; j; j = j->mYoung) {
                MR::hideJointAndChildren(j);
            }
        }
    }

    void hideJointAndChildren(J3DModel* pModel, const char* pName) {
        MR::hideJointAndChildren(MR::getJoint(pModel, pName));
    }

    void hideJointAndChildren(const LiveActor* pActor, const char* pName) {
        hideJointAndChildren(MR::getJ3DModel(pActor), pName);
    }

    void showJoint(J3DJoint* pJoint) {
        for (J3DMaterial* mat = pJoint->mMesh; mat != nullptr; mat = mat->_4) {
            mat->mShape->mFlags &= ~1;
        }
    }

    void showJoint(J3DModel* pModel, const char* pName) {
        MR::showJoint(MR::getJoint(pModel, pName));
    }

    void showJoint(const LiveActor* pActor, const char* pName) {
        showJoint(MR::getJ3DModel(pActor), pName);
    }

    void showJointAndChildren(J3DJoint* pJoint) {
        MR::showJoint(pJoint);
        J3DJoint *j, *i;
        for (i = pJoint->mChild; i; i = i->mChild) {
            for (j = i; j; j = j->mYoung) {
                MR::showJointAndChildren(j);
            }
        }
    }

    void showJointAndChildren(J3DModel* pModel, const char* pName) {
        MR::showJointAndChildren(MR::getJoint(pModel, pName));
    }

    void showJointAndChildren(const LiveActor* pActor, const char* pName) {
        showJointAndChildren(MR::getJ3DModel(pActor), pName);
    }

    f32 getJointTransX(const J3DJoint* pJoint) {
        return pJoint->mTransformInfo.mTranslate.x;
    }

    f32 getJointTransY(const J3DJoint* pJoint) {
        return pJoint->mTransformInfo.mTranslate.y;
    }

    f32 getJointTransZ(const J3DJoint* pJoint) {
        return pJoint->mTransformInfo.mTranslate.z;
    }

    /*
    J3DJoint* searchChildJoint(J3DJoint *a1, J3DJoint *a2) {
        J3DJoint* v3 = a1;
        if (a1 == nullptr) {
            return nullptr;
        }

        if (a1 == a2) {
            return (J3DJoint*)-1;
        }

        J3DJoint* child = a1->mChild;
        if (child != a2) {
            while (child != nullptr) {
                child = child->mYoung;

                if (child == a2) {
                    return a1;
                }
            }

            a1 = searchChildJoint(a1->mYoung, a2);

            if (a1 != nullptr) {
                return searchChildJoint(v3->mYoung, a2);
            }
        }

        return a1;
    }

    J3DJoint* getParentJoint(J3DModelData *pModelData, J3DJoint *pJoint) {
        
    }
    */
}; // namespace MR
