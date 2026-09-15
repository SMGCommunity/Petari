#include "Game/Animation/XanimeCore.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3DGraphAnimator/J3DMtxBuffer.hpp>
#include <JSystem/JMath/JMath.hpp>

TVec3f (TVec3f::* XanimeCore_FORCE_EMIT())(const TVec3f&) const {
    (void)1.0f;
    (void)0.0f;
    TVec3f* pVectors = new TVec3f[1];
    delete[] pVectors;
    return &TVec3f::operator-;
}

void XanimeTrack::init() {
    _0 = nullptr;
    mWeight = 0.0f;
    _C = 0;
    _8 = 0.0f;
}

XjointInfo::XjointInfo() {
    _0._0.set< f32 >(1.0f, 1.0f, 1.0f);
    _0._C.x = 0.0f;
    _0._C.y = 0.0f;
    _0._C.z = 0.0f;
    _0.mRotation.z = 0.0f;
    _0.mRotation.y = 0.0f;
    _0.mRotation.x = 0.0f;
    _0.mRotation.w = 1.0f;
    _28 = _0;
    _5C = 1.0f;
    _60 = 1.0f;
    _50.zero();
}

XjointTransform::XjointTransform() {
    _0 = nullptr;
    _4 = 0xFFFF;
    mScale.set(1.0f);
    _14.set(1.0f);
    _64 = nullptr;
    _6C = nullptr;
    _68 = 0;
    _20.x = 0.0f;
    _20.y = 0.0f;
    _20.z = 0.0f;
    _2C.x = 0.0f;
    _2C.y = 0.0f;
    _2C.z = 0.0f;
    _38.x = 0.0f;
    _38.y = 0.0f;
    _38.z = 0.0f;
    _44.mScale.x = 0.0f;
    _44.mScale.y = 0.0f;
    _44.mScale.z = 0.0f;
    _44.mRotation.x = 0;
    _44.mRotation.y = 0;
    _44.mRotation.z = 0;
    _44._12 = 0;
    _44.mTranslate.x = 0.0f;
    _44.mTranslate.y = 0.0f;
    _44.mTranslate.z = 0.0f;
}

void XanimeCore::shareJointTransform(const XanimeCore* pOther) {
    mTransformList = pOther->mTransformList;
}

void XanimeCore::enableJointTransform(J3DModelData* pModelData) {
    mTransformList = new XjointTransform[mJointCount];

    for (u32 i = 0; i < mJointCount; i++) {
        J3DJoint* pJoint = pModelData->getJointNodePointer(i);
        mTransformList[i]._0 = pJoint;
        mTransformList[i]._44 = pJoint->getTransformInfo();

        if (mTransformList[i]._4 == 0xFFFF) {
            for (u32 j = 0; j < mJointCount; j++) {
                if (mTransformList[j]._0 != nullptr) {
                    if (mTransformList[j]._0->getChild() == pJoint) {
                        mTransformList[i]._4 = j;
                        break;
                    }

                    if (mTransformList[j]._0->getYounger() == pJoint && mTransformList[j]._4 != 0xFFFF) {
                        mTransformList[i]._4 = mTransformList[j]._4;
                        break;
                    }
                }
            }
        }
    }
}

void XanimeCore::reconfigJointTransform(J3DModelData* pModelData) {
    for (u32 i = 0; i < mJointCount; i++) {
        J3DJoint* pJoint = pModelData->getJointNodePointer(i);
        mTransformList[i]._0 = pJoint;
        mTransformList[i]._44 = pJoint->getTransformInfo();
    }
}

void XanimeCore::initMember(u32 trackCount) {
    _1C = 1.0f;
    _20 = 1.0f;
    mFrameRatio = 0.0f;
    _28 = 0;
    _29 = 0;
    _6 = 0;
    _C = 0;
    mTrackList = new XanimeTrack[trackCount];

    s32 curTrack = 0;

    while (curTrack < trackCount) {
        mTrackList[curTrack].init();
        curTrack++;
    }

    setWeight(0, 1.0f);
}

XanimeCore::XanimeCore(u32 trackCount, u32 jointCount, u8 a3) {
    _4 = a3;
    mTrackCount = trackCount;
    mJointCount = jointCount;
    mJointList = new XjointInfo[jointCount];
    mTransformList = nullptr;
    initMember(trackCount);
}

XanimeCore::XanimeCore(u32 trackCount, XanimeCore* pOtherCore) {
    mTrackCount = trackCount;
    mJointCount = pOtherCore->mJointCount;
    _4 = pOtherCore->_4;
    mJointList = pOtherCore->mJointList;
    mTransformList = pOtherCore->mTransformList;
    initMember(trackCount);
}

void XanimeCore::doFreeze() {
    _28 = 1;
    _1C = 0.0f;
}

void XanimeCore::setBck(u32 index, J3DAnmTransform* pAnm) {
    mTrackList[index]._0 = pAnm;
    mTrackList[index].setFrame(0.0f);
    mTrackList[index]._C = 0;
}

void XanimeCore::setWeight(u8 idx, f32 weight) {
    mTrackList[idx].mWeight = weight;
}

void XanimeCore::calcBlend(TVec3f* pScale, TVec3f* pTranslate) {
    u16 joint = getJoint()->getJntNo();
    MtxPtr pMtx = getMtxBuffer()->getAnmMtx(joint);
    J3DTransformInfo transform;
    Quaternion rotation;
    f32 inverseWeight;
    f32 weight;
    f32 accumulatedWeight = 0.0f;
    pScale->z = 0.0f;
    pScale->y = 0.0f;
    pScale->x = 0.0f;
    pTranslate->z = 0.0f;
    pTranslate->y = 0.0f;
    pTranslate->x = 0.0f;
    rotation.z = 0.0f;
    rotation.y = 0.0f;
    rotation.x = 0.0f;
    rotation.w = 1.0f;

    f32 totalWeight = 0.0f;

    for (s32 i = 0; i < mTrackCount; i++) {
        if (mTrackList[i]._0 != nullptr) {
            totalWeight += mTrackList[i].mWeight;
        }
    }

    if (totalWeight == 0.0f) {
        *pScale = mJointList[joint]._28._0;
        *pTranslate = mJointList[joint]._28._C;
        PSMTXQuat(pMtx, &mJointList[joint]._28.mRotation);
        return;
    }

    inverseWeight = 1.0f / totalWeight;

    for (s32 i = 0; i < mTrackCount; i++) {
        XanimeTrack* pTrack = &mTrackList[i];

        if (pTrack->_0 != nullptr && pTrack->mWeight != 0.0f) {
            pTrack->_0->getTransform(joint, &transform);
            Quaternion trackRotation;
            JMAEulerToQuat(transform.mRotation.x, transform.mRotation.y, transform.mRotation.z, &trackRotation);

            weight = inverseWeight * pTrack->mWeight;
            *pScale += reinterpret_cast< const TVec3f& >(transform.mScale) * weight;
            *pTranslate += reinterpret_cast< const TVec3f& >(transform.mTranslate) * weight;
            accumulatedWeight += weight;
            JMAQuatLerp(&rotation, &trackRotation, weight / accumulatedWeight, &rotation);
        }
    }

    if (_29 != 0) {
        mJointList[joint]._0 = mJointList[joint]._28;
    }

    mJointList[joint]._50 = *pTranslate;
    f32 blend = _1C;

    if (blend < 1.0f) {
        const TVec3f* pFrozenScale = &mJointList[joint]._0._0;
        const TVec3f* pFrozenTranslate = &mJointList[joint]._0._C;
        XjointInfo& rJoint = mJointList[joint];
        *pScale = *pFrozenScale * (1.0f - blend) + *pScale * blend;
        *pTranslate = *pFrozenTranslate * (1.0f - blend) + *pTranslate * blend;
        JMAQuatLerp(&rJoint._0.mRotation, &rotation, blend, &rotation);
    }

    if (_20 != 1.0f) {
        TVec3f* pPreviousScale = &mJointList[joint]._28._0;
        TVec3f* pPreviousTranslate = &mJointList[joint]._28._C;
        XjointInfo& rJoint = mJointList[joint];
        *pScale = *pPreviousScale * (1.0f - _20) + *pScale * _20;
        *pTranslate = *pPreviousTranslate * (1.0f - _20) + *pTranslate * _20;
        JMAQuatLerp(&rJoint._28.mRotation, &rotation, _20, &rotation);
    }

    mJointList[joint]._28._0 = *pScale;
    mJointList[joint]._28._C = *pTranslate;
    mJointList[joint]._28.mRotation = rotation;
    PSMTXQuat(pMtx, &rotation);
}

void XanimeCore::calcSingle(TVec3f* pScale, TVec3f* pTranslate) {
    u16 joint = getJoint()->getJntNo();
    MtxPtr pMtx = getMtxBuffer()->getAnmMtx(joint);
    J3DTransformInfo transform;
    Quaternion rotation;

    if (mTrackList[0]._0 == nullptr) {
        *pScale = mJointList[joint]._28._0;
        *pTranslate = mJointList[joint]._28._C;
        PSMTXQuat(pMtx, &mJointList[joint]._28.mRotation);
        return;
    }

    mTrackList[0]._0->getTransform(joint, &transform);
    JMAEulerToQuat(transform.mRotation.x, transform.mRotation.y, transform.mRotation.z, &rotation);
    *pScale = reinterpret_cast< const TVec3f& >(transform.mScale);
    *pTranslate = reinterpret_cast< const TVec3f& >(transform.mTranslate);

    if (_29 != 0) {
        mJointList[joint]._0 = mJointList[joint]._28;
    }

    mJointList[joint]._50 = *pTranslate;
    f32 blend = _1C;

    if (blend < 1.0f) {
        XjointInfo* pJoints = mJointList;
        MR::vecBlend(pJoints[joint]._0._0, *pScale, pScale, blend);
        MR::vecBlend(pJoints[joint]._0._C, *pTranslate, pTranslate, blend);
        JMAQuatLerp(&pJoints[joint]._0.mRotation, &rotation, blend, &rotation);
    }

    mJointList[joint]._28._0 = *pScale;
    mJointList[joint]._28._C = *pTranslate;
    mJointList[joint]._28.mRotation = rotation;
    PSMTXQuat(pMtx, &rotation);
}

void XanimeCore::calcBlendSpecial() {
    u16 joint = getJoint()->getJntNo();
    J3DTransformInfo transform;
    Quaternion rotation;
    TVec3f scale(0.0f, 0.0f, 0.0f);
    TVec3f translate(0.0f, 0.0f, 0.0f);
    f32 inverseWeight;
    f32 weight;
    f32 accumulatedWeight = 0.0f;
    rotation.z = 0.0f;
    rotation.y = 0.0f;
    rotation.x = 0.0f;
    rotation.w = 1.0f;

    f32 totalWeight = 0.0f;

    for (s32 i = 0; i < mTrackCount; i++) {
        if (mTrackList[i]._0 != nullptr) {
            totalWeight += mTrackList[i].mWeight;
        }
    }

    if (totalWeight == 0.0f) {
        return;
    }

    inverseWeight = 1.0f / totalWeight;

    for (s32 i = 0; i < mTrackCount; i++) {
        XanimeTrack* pTrack = &mTrackList[i];

        if (pTrack->_0 != nullptr && pTrack->mWeight != 0.0f) {
            pTrack->_0->getTransform(joint, &transform);
            Quaternion trackRotation;
            JMAEulerToQuat(transform.mRotation.x, transform.mRotation.y, transform.mRotation.z, &trackRotation);

            weight = inverseWeight * pTrack->mWeight;
            scale += reinterpret_cast< const TVec3f& >(transform.mScale) * weight;
            translate += reinterpret_cast< const TVec3f& >(transform.mTranslate) * weight;
            accumulatedWeight += weight;
            JMAQuatLerp(&rotation, &trackRotation, weight / accumulatedWeight, &rotation);
        }
    }

    if (_29 != 0) {
        mJointList[joint]._0 = mJointList[joint]._28;
    }

    mJointList[joint]._50 = translate;
    f32 blend = _1C;

    if (mJointList[joint]._5C != 1.0f) {
        mJointList[joint]._5C += mJointList[joint]._60;
        mJointList[joint]._5C = MR::clamp(mJointList[joint]._5C, 0.0f, 1.0f);
        blend = mJointList[joint]._5C;
    }

    if (blend < 1.0f) {
        const TVec3f* pFrozenScale = &mJointList[joint]._0._0;
        const TVec3f* pFrozenTranslate = &mJointList[joint]._0._C;
        XjointInfo& rJoint = mJointList[joint];
        scale = *pFrozenScale * (1.0f - blend) + scale * blend;
        translate = *pFrozenTranslate * (1.0f - blend) + translate * blend;
        JMAQuatLerp(&rJoint._0.mRotation, &rotation, blend, &rotation);
    }

    if (_20 != 1.0f) {
        TVec3f* pPreviousScale = &mJointList[joint]._28._0;
        TVec3f* pPreviousTranslate = &mJointList[joint]._28._C;
        XjointInfo& rJoint = mJointList[joint];
        scale = *pPreviousScale * (1.0f - _20) + scale * _20;
        translate = *pPreviousTranslate * (1.0f - _20) + translate * _20;
        JMAQuatLerp(&rJoint._28.mRotation, &rotation, _20, &rotation);
    }

    mJointList[joint]._28._0 = scale;
    mJointList[joint]._28._C = translate;
    mJointList[joint]._28.mRotation = rotation;
}

void XanimeCore::updateFrame() {
    for (u32 i = 0; i < mTrackCount; i++) {
        if (mTrackList[i]._0 != nullptr) {
            if (mTrackList[i]._C != 0) {
                f32 frame = mTrackList[i]._8;
                mTrackList[i]._0->setFrame(frame * mTrackList[i]._0->getFrameMax());
            } else {
                mTrackList[i]._0->setFrame(mFrameRatio * mTrackList[i]._0->getFrameMax());
            }
        }
    }

    if (_28 != 0) {
        _28 = 0;
        _29 = 1;
    } else {
        _29 = 0;
    }
}

void XanimeCore::calcScaleBlendMaya(const TVec3f& rScale, const TVec3f& rTranslate) {
    J3DJoint* pJoint = getJoint();
    u16 joint = pJoint->getJntNo();
    J3DMtxBuffer* pMtxBuffer = getMtxBuffer();
    MtxPtr pMtx = pMtxBuffer->getAnmMtx(joint);

    if (mTransformList != nullptr) {
        if (mTransformList[joint]._64 != nullptr) {
            PSMTXConcat(pMtx, mTransformList[joint]._64, pMtx);
        }

        if (mTransformList[joint]._68 != 0) {
            PSMTXConcat(pMtx, reinterpret_cast< MtxPtr >(mTransformList[joint]._68), pMtx);
        }
    }

    if (mTransformList != nullptr && mTransformList[joint]._4 != 0xFFFF && mTransformList[mTransformList[joint]._4]._68 != 0) {
        Mtx inverse;
        PSMTXInverse(reinterpret_cast< MtxPtr >(mTransformList[mTransformList[joint]._4]._68), inverse);
        PSMTXConcat(inverse, pMtx, pMtx);
    }

    pMtx[0][3] = rTranslate.x;
    pMtx[1][3] = rTranslate.y;
    pMtx[2][3] = rTranslate.z;

    if (mTransformList != nullptr) {
        pMtx[0][3] += mTransformList[joint]._2C.x;
        pMtx[1][3] += mTransformList[joint]._2C.y;
        pMtx[2][3] += mTransformList[joint]._2C.z;
    }

    TVec3f scale(rScale);

    if (mTransformList != nullptr) {
        scale.x *= mTransformList[joint].mScale.x * mTransformList[joint]._14.x;
        scale.y *= mTransformList[joint].mScale.y * mTransformList[joint]._14.y;
        scale.z *= mTransformList[joint].mScale.z * mTransformList[joint]._14.z;
    }

    if (scale.x == 1.0f && scale.y == 1.0f && scale.z == 1.0f) {
        pMtxBuffer->setScaleFlag(joint, 1);
    } else {
        pMtxBuffer->setScaleFlag(joint, 0);
        JMAMTXApplyScale(pMtx, pMtx, scale.x, scale.y, scale.z);
    }

    if (mTransformList != nullptr && mTransformList[joint]._4 != 0xFFFF) {
        if (mTransformList[mTransformList[joint]._4].mScale.x != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(mTransformList[mTransformList[joint]._4].mScale.x);
            pMtx[0][0] *= inverseScale;
            pMtx[0][1] *= inverseScale;
            pMtx[0][2] *= inverseScale;
        }

        if (mTransformList[mTransformList[joint]._4].mScale.y != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(mTransformList[mTransformList[joint]._4].mScale.y);
            pMtx[1][0] *= inverseScale;
            pMtx[1][1] *= inverseScale;
            pMtx[1][2] *= inverseScale;
        }

        if (mTransformList[mTransformList[joint]._4].mScale.z != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(mTransformList[mTransformList[joint]._4].mScale.z);
            pMtx[2][0] *= inverseScale;
            pMtx[2][1] *= inverseScale;
            pMtx[2][2] *= inverseScale;
        }
    }

    if (pJoint->getScaleCompensate() == 1) {
        if (J3DSys::mParentS.x != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(J3DSys::mParentS.x);
            pMtx[0][0] *= inverseScale;
            pMtx[0][1] *= inverseScale;
            pMtx[0][2] *= inverseScale;
        }

        if (J3DSys::mParentS.y != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(J3DSys::mParentS.y);
            pMtx[1][0] *= inverseScale;
            pMtx[1][1] *= inverseScale;
            pMtx[1][2] *= inverseScale;
        }

        if (J3DSys::mParentS.z != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(J3DSys::mParentS.z);
            pMtx[2][0] *= inverseScale;
            pMtx[2][1] *= inverseScale;
            pMtx[2][2] *= inverseScale;
        }
    }

    PSMTXConcat(J3DSys::mCurrentMtx, pMtx, J3DSys::mCurrentMtx);

    if (mTransformList != nullptr && mTransformList[joint]._6C != nullptr) {
        TVec3f translate;
        MR::extractMtxTrans(J3DSys::mCurrentMtx, &translate);
        MR::setMtxTrans(J3DSys::mCurrentMtx, 0.0f, 0.0f, 0.0f);
        PSMTXConcat(mTransformList[joint]._6C, J3DSys::mCurrentMtx, J3DSys::mCurrentMtx);
        MR::setMtxTrans(J3DSys::mCurrentMtx, translate.x, translate.y, translate.z);
    }

    if (mTransformList != nullptr) {
        J3DSys::mCurrentMtx[0][3] += mTransformList[joint]._38.x;
        J3DSys::mCurrentMtx[1][3] += mTransformList[joint]._38.y;
        J3DSys::mCurrentMtx[2][3] += mTransformList[joint]._38.z;
    }

    PSMTXCopy(J3DSys::mCurrentMtx, pMtx);

    if (mTransformList != nullptr) {
        pMtx[0][3] += mTransformList[joint]._20.x;
        pMtx[1][3] += mTransformList[joint]._20.y;
        pMtx[2][3] += mTransformList[joint]._20.z;
    }

    J3DSys::mParentS.x = rScale.x;
    J3DSys::mParentS.y = rScale.y;
    J3DSys::mParentS.z = rScale.z;
}

void XanimeCore::calcScaleBlendMayaNoTransform(const TVec3f& rScale, const TVec3f& rTranslate) {
    J3DJoint* pJoint = getJoint();
    u16 joint = pJoint->getJntNo();
    J3DMtxBuffer* pMtxBuffer = getMtxBuffer();
    MtxPtr pMtx = pMtxBuffer->getAnmMtx(joint);

    pMtx[0][3] = rTranslate.x;
    pMtx[1][3] = rTranslate.y;
    pMtx[2][3] = rTranslate.z;

    TVec3f scale(rScale);

    if (scale.x == 1.0f && scale.y == 1.0f && scale.z == 1.0f) {
        pMtxBuffer->setScaleFlag(joint, 1);
    } else {
        pMtxBuffer->setScaleFlag(joint, 0);
        JMAMTXApplyScale(pMtx, pMtx, scale.x, scale.y, scale.z);
    }

    if (pJoint->getScaleCompensate() == 1) {
        if (J3DSys::mParentS.x != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(J3DSys::mParentS.x);
            pMtx[0][0] *= inverseScale;
            pMtx[0][1] *= inverseScale;
            pMtx[0][2] *= inverseScale;
        }

        if (J3DSys::mParentS.y != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(J3DSys::mParentS.y);
            pMtx[1][0] *= inverseScale;
            pMtx[1][1] *= inverseScale;
            pMtx[1][2] *= inverseScale;
        }

        if (J3DSys::mParentS.z != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(J3DSys::mParentS.z);
            pMtx[2][0] *= inverseScale;
            pMtx[2][1] *= inverseScale;
            pMtx[2][2] *= inverseScale;
        }
    }

    PSMTXConcat(J3DSys::mCurrentMtx, pMtx, J3DSys::mCurrentMtx);
    PSMTXCopy(J3DSys::mCurrentMtx, pMtx);

    J3DSys::mParentS.x = rScale.x;
    J3DSys::mParentS.y = rScale.y;
    J3DSys::mParentS.z = rScale.z;
}

void XanimeCore::calcScaleBlendSI(const TVec3f& rScale, const TVec3f& rTranslate) {
    Vec& rCurrentScale = J3DSys::mCurrentS;
    J3DJoint* pJoint = getJoint();
    u16 joint = pJoint->getJntNo();
    J3DMtxBuffer* pMtxBuffer = getMtxBuffer();
    MtxPtr pMtx = pMtxBuffer->getAnmMtx(joint);

    if (mTransformList != nullptr) {
        if (mTransformList[joint]._64 != nullptr) {
            PSMTXConcat(pMtx, mTransformList[joint]._64, pMtx);
        }

        if (mTransformList[joint]._68 != 0) {
            PSMTXConcat(pMtx, reinterpret_cast< MtxPtr >(mTransformList[joint]._68), pMtx);
        }
    }

    TVec3f scale(rScale);

    if (mTransformList != nullptr) {
        scale.x *= mTransformList[joint].mScale.x * mTransformList[joint]._14.x;
        scale.y *= mTransformList[joint].mScale.y * mTransformList[joint]._14.y;
        scale.z *= mTransformList[joint].mScale.z * mTransformList[joint]._14.z;
    }

    if (scale.x == 1.0f && scale.y == 1.0f && scale.z == 1.0f) {
        pMtxBuffer->setScaleFlag(joint, 1);
    } else {
        pMtxBuffer->setScaleFlag(joint, 0);
        JMAMTXApplyScale(pMtx, pMtx, scale.x, scale.y, scale.z);
    }

    if (mTransformList != nullptr && mTransformList[joint]._4 != 0xFFFF) {
        if (mTransformList[mTransformList[joint]._4].mScale.x != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(mTransformList[mTransformList[joint]._4].mScale.x);
            pMtx[0][0] *= inverseScale;
            pMtx[0][1] *= inverseScale;
            pMtx[0][2] *= inverseScale;
        }

        if (mTransformList[mTransformList[joint]._4].mScale.y != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(mTransformList[mTransformList[joint]._4].mScale.y);
            pMtx[1][0] *= inverseScale;
            pMtx[1][1] *= inverseScale;
            pMtx[1][2] *= inverseScale;
        }

        if (mTransformList[mTransformList[joint]._4].mScale.z != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(mTransformList[mTransformList[joint]._4].mScale.z);
            pMtx[2][0] *= inverseScale;
            pMtx[2][1] *= inverseScale;
            pMtx[2][2] *= inverseScale;
        }
    }

    if (pJoint->getScaleCompensate() == 1) {
        if (J3DSys::mParentS.x != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(J3DSys::mParentS.x);
            pMtx[0][0] *= inverseScale;
            pMtx[0][1] *= inverseScale;
            pMtx[0][2] *= inverseScale;
        }

        if (J3DSys::mParentS.y != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(J3DSys::mParentS.y);
            pMtx[1][0] *= inverseScale;
            pMtx[1][1] *= inverseScale;
            pMtx[1][2] *= inverseScale;
        }

        if (J3DSys::mParentS.z != 1.0f) {
            f32 inverseScale = JMath::fastReciprocal(J3DSys::mParentS.z);
            pMtx[2][0] *= inverseScale;
            pMtx[2][1] *= inverseScale;
            pMtx[2][2] *= inverseScale;
        }
    }

    pMtx[0][3] = rTranslate.x * rCurrentScale.x;
    pMtx[1][3] = rTranslate.y * rCurrentScale.y;
    pMtx[2][3] = rTranslate.z * rCurrentScale.z;

    if (mTransformList != nullptr) {
        pMtx[0][3] += mTransformList[joint]._2C.x;
        pMtx[1][3] += mTransformList[joint]._2C.y;
        pMtx[2][3] += mTransformList[joint]._2C.z;
    }

    PSMTXConcat(J3DSys::mCurrentMtx, pMtx, J3DSys::mCurrentMtx);

    rCurrentScale.x *= rScale.x;
    rCurrentScale.y *= rScale.y;
    rCurrentScale.z *= rScale.z;

    if (rCurrentScale.x == 1.0f && rCurrentScale.y == 1.0f && rCurrentScale.z == 1.0f) {
        pMtxBuffer->setScaleFlag(joint, 1);
        PSMTXCopy(J3DSys::mCurrentMtx, pMtx);
    } else {
        pMtxBuffer->setScaleFlag(joint, 0);
        JMAMTXApplyScale(J3DSys::mCurrentMtx, pMtx, rCurrentScale.x, rCurrentScale.y, rCurrentScale.z);
        pMtx[0][3] = J3DSys::mCurrentMtx[0][3];
        pMtx[1][3] = J3DSys::mCurrentMtx[1][3];
        pMtx[2][3] = J3DSys::mCurrentMtx[2][3];
    }

    if (mTransformList != nullptr) {
        pMtx[0][3] += mTransformList[joint]._20.x;
        pMtx[1][3] += mTransformList[joint]._20.y;
        pMtx[2][3] += mTransformList[joint]._20.z;
    }
}

void XanimeCore::calcScaleBlendBasic(const TVec3f& rScale, const TVec3f& rTranslate) {
    J3DJoint* pJoint = getJoint();
    u16 joint = pJoint->getJntNo();
    J3DMtxBuffer* pMtxBuffer = getMtxBuffer();
    MtxPtr pMtx = pMtxBuffer->getAnmMtx(joint);

    TVec3f& rCurrentScale = reinterpret_cast< TVec3f& >(J3DSys::mCurrentS);
    pMtx[0][3] = rTranslate.x;
    pMtx[1][3] = rTranslate.y;
    pMtx[2][3] = rTranslate.z;

    rCurrentScale.x *= rScale.x;
    rCurrentScale.y *= rScale.y;
    rCurrentScale.z *= rScale.z;

    if (rCurrentScale.x == 1.0f && rCurrentScale.y == 1.0f && rCurrentScale.z == 1.0f) {
        pMtxBuffer->setScaleFlag(joint, 1);
    } else {
        pMtxBuffer->setScaleFlag(joint, 0);
        JMAMTXApplyScale(pMtx, pMtx, rScale.x, rScale.y, rScale.z);
    }

    PSMTXConcat(J3DSys::mCurrentMtx, pMtx, J3DSys::mCurrentMtx);
    PSMTXCopy(J3DSys::mCurrentMtx, pMtx);
}

void XanimeCore::calcScaleBlendSpecial() {
    u16 joint = getJoint()->getJntNo();
    MtxPtr pMtx = getMtxBuffer()->getAnmMtx(joint);
    TVec3f scale;
    TVec3f translate;
    scale = mJointList[joint]._28._0;
    translate = mJointList[joint]._28._C;
    PSMTXQuat(pMtx, &mJointList[joint]._28.mRotation);
    calcScaleBlendMaya(scale, translate);
}

void XanimeCore::freezeCopy(J3DModelData* pModelData, XanimeCore* pOther, u32 joint, u32 duration) {
    J3DJoint* pChild = pModelData->getJointNodePointer(joint)->getChild();

    if (pChild != nullptr) {
        freezeCopy(pModelData, pOther, pChild->getJntNo(), duration);

        while (true) {
            pChild = pChild->getYounger();
            if (pChild == nullptr) {
                break;
            }

            freezeCopy(pModelData, pOther, pChild->getJntNo(), duration);
        }
    }

    pOther->mJointList[joint]._0 = mJointList[joint]._28;
    pOther->mJointList[joint]._5C = 0.0f;
    f32 rate = 1.0f;

    if (duration != 0) {
        rate = 1.0f / duration;
    }

    pOther->mJointList[joint]._60 = rate;
}

void XanimeCore::initT(J3DModelData* pModelData) {
    for (u32 i = 0; i < mJointCount; i++) {
        const J3DTransformInfo& rTransform = pModelData->getJointNodePointer(i)->getTransformInfo();
        Quaternion rotation;
        JMAEulerToQuat(rTransform.mRotation.x, rTransform.mRotation.y, rTransform.mRotation.z, &rotation);
        mJointList[i]._0._0 = reinterpret_cast< const TVec3f& >(rTransform.mScale);
        mJointList[i]._0._C = reinterpret_cast< const TVec3f& >(rTransform.mTranslate);
        mJointList[i]._0.mRotation = rotation;
        mJointList[i]._28._0 = mJointList[i]._0._0;
        mJointList[i]._28._C = mJointList[i]._0._C;
        mJointList[i]._28.mRotation = mJointList[i]._0.mRotation;
    }
}

void XanimeCore::fixT(TVec3f* pTranslate) {
    u16 joint = getJoint()->getJntNo();
    J3DModelData* pModelData = j3dSys.getModel()->getModelData();

    if (joint != 0 && joint != _C) {
        *pTranslate = reinterpret_cast< const TVec3f& >(pModelData->getJointNodePointer(joint)->getTransformInfo().mTranslate);
    }
}

XanimeCore::~XanimeCore() {
}

XtransformInfo::XtransformInfo() {
    _0.zero();
    _C.zero();
    mRotation.z = 0.0f;
    mRotation.y = 0.0f;
    mRotation.x = 0.0f;
    mRotation.w = 1.0f;
}

void XanimeCore::calc() {
    j3dSys.mCurrentMtxCalc = this;

    if (_6 == 1) {
        calcBlendSpecial();
        return;
    }

    if (_6 == 2) {
        calcScaleBlendSpecial();
        return;
    }

    TVec3f scale;
    TVec3f translate;

    if (mTrackCount == 1) {
        calcSingle(&scale, &translate);
    } else {
        calcBlend(&scale, &translate);
    }

    if (_6 == 3) {
        fixT(&translate);
    }

    switch (_4) {
    case 1:
        calcScaleBlendSI(scale, translate);
        break;
    case 0:
        if (mTransformList == nullptr) {
            calcScaleBlendBasic(scale, translate);
            break;
        }
    case 2:
        if (mTransformList != nullptr) {
            calcScaleBlendMaya(scale, translate);
        } else {
            calcScaleBlendMayaNoTransform(scale, translate);
        }
        break;
    }
}

void XanimeCore::init(const Vec& rScale, const Mtx& rMtx) {
    switch (_4) {
    case 0:
    case 1:
    case 2:
        J3DMtxCalcJ3DSysInitMaya::init(rScale, rMtx);
        break;
    }
}
