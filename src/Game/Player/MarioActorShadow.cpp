#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/J3DModelX.hpp"
#include "Game/Player/JetTurtleShadow.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioShadow.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Player/ModelHolder.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/FootPrint.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/J3DGraphBase/J3DSys.hpp>

void MarioActor::initShadow() {
    _A08 = 1;

    if (gIsLuigi) {
        _A10 = new ModelHolder("LuigiShadow", true);
    } else {
        _A10 = new ModelHolder("MarioShadow", true);
    }

    _A10->initWithoutIter();

    _A14 = reinterpret_cast< J3DModelX* >(MR::getJ3DModel(_A10));
    J3DModelX* source = mModels[0];
    J3DModelX* shadow = _A14;
    shadow->_DD = source->_DD;

    for (u32 idx = 0; idx < shadow->_DD; idx++) {
        shadow->mExtraMtxBuffer[idx] = source->mExtraMtxBuffer[idx];
    }
}

void MarioActor::calcViewReflectionModel() {
    J3DModelX* model = getJ3DModel();
    TPos3f matrix;
    PSMTXCopy(getJ3DModel()->getBaseTRMtx(), matrix.toMtxPtr());
    TPos3f offset;
    PSMTXIdentity(offset.toMtxPtr());
    f32 distance = 10.0f;
    if (isAnimationRun("ターンジャンプ")) {
        distance = 25.0f;
    }
    if (_A08 != 6 && _A08 != 7) {
        _A24 = 0;
    }
    if (_A08 != 3) {
        _A25 = 0;
    }

    TVec3f position;
    switch (_A08) {
    case 2: {
        matrix.mMtx[0][1] = -matrix.mMtx[0][1];
        matrix.mMtx[1][1] = -matrix.mMtx[1][1];
        matrix.mMtx[2][1] = -matrix.mMtx[2][1];
        position = mMario->mShadowPos - *mMario->_45C->getNormal(0) * distance;
        TVec3f root;
        getRealPos("All_Root", &root);
        TVec3f translation(mPosition - root);
        PSMTXTrans(offset.toMtxPtr(), translation.x, translation.y, translation.z);
        updateReflectAlphaDL(_A09);
        break;
    }
    case 3: {
        TVec3f x;
        x.set(matrix.mMtx[0][0], matrix.mMtx[1][0], matrix.mMtx[2][0]);
        TVec3f y;
        y.set(matrix.mMtx[0][1], matrix.mMtx[1][1], matrix.mMtx[2][1]);
        TVec3f z;
        z.set(matrix.mMtx[0][2], matrix.mMtx[1][2], matrix.mMtx[2][2]);
        const TVec3f& normal = mMario->mSwim->mSurfacePos;
        f32 dx = MR::vecKillElement(x, normal, &x);
        f32 dy = MR::vecKillElement(y, normal, &y);
        f32 dz = MR::vecKillElement(z, normal, &z);
        x -= normal * dx;
        y -= normal * dy;
        z -= normal * dz;
        matrix.mMtx[0][0] = x.x;
        matrix.mMtx[1][0] = x.y;
        matrix.mMtx[2][0] = x.z;
        matrix.mMtx[0][1] = y.x;
        matrix.mMtx[1][1] = y.y;
        matrix.mMtx[2][1] = y.z;
        matrix.mMtx[0][2] = z.x;
        matrix.mMtx[1][2] = z.y;
        matrix.mMtx[2][2] = z.z;
        position = mMario->mSwim->mSurfaceNorm - normal * distance;
        f32 depth = -mMario->mSwim->mWaterDepth;
        f32 factor;
        if (depth < 40.0f) {
            factor = 0.0f;
        } else if (depth < 200.0f) {
            factor = (depth - 40.0f) / 160.0f;
        } else {
            factor = 1.0f;
        }
        f32 alpha = mCamDirZ.dot(normal);
        MR::clamp01(&alpha);
        alpha *= 1.414f;
        MR::clamp01(&alpha);
        alpha *= 64.0f;
        f32 target = factor * alpha;
        if (_A25 > static_cast< u8 >(target)) {
            _A25--;
        } else if (_A25 < static_cast< u8 >(target)) {
            _A25++;
        }
        updateSimpleAlphaDL(_A25);
        _B18 = position;
        break;
    }
    case 6:
    case 7: {
        TVec3f x;
        x.set(matrix.mMtx[0][0], matrix.mMtx[1][0], matrix.mMtx[2][0]);
        TVec3f y;
        y.set(matrix.mMtx[0][1], matrix.mMtx[1][1], matrix.mMtx[2][1]);
        TVec3f z;
        z.set(matrix.mMtx[0][2], matrix.mMtx[1][2], matrix.mMtx[2][2]);
        TVec3f normal(mMario->mSwim->_178 - mPosition);
        MR::normalize(&normal);
        f32 dx = MR::vecKillElement(x, normal, &x);
        f32 dy = MR::vecKillElement(y, normal, &y);
        f32 dz = MR::vecKillElement(z, normal, &z);
        x -= normal * dx;
        y -= normal * dy;
        z -= normal * dz;
        matrix.mMtx[0][0] = x.x;
        matrix.mMtx[1][0] = x.y;
        matrix.mMtx[2][0] = x.z;
        matrix.mMtx[0][1] = y.x;
        matrix.mMtx[1][1] = y.y;
        matrix.mMtx[2][1] = y.z;
        matrix.mMtx[0][2] = z.x;
        matrix.mMtx[1][2] = z.y;
        matrix.mMtx[2][2] = z.z;
        position = mMario->mSwim->_178 - normal * distance;
        f32 edgeDistance = mMario->mSwim->getWaterEdgeDist();
        f32 factor = 0.0f;
        if (edgeDistance < 0.0f) {
        } else if (edgeDistance < 1000.0f) {
            factor = 0.5f + 0.5f * (edgeDistance / 1000.0f);
        }
        f32 alpha = mCamDirZ.dot(normal);
        MR::clamp01(&alpha);
        alpha *= 1.414f;
        MR::clamp01(&alpha);
        alpha *= 127.0f;
        f32 target = factor * alpha;
        if (_A24 > static_cast< u8 >(target)) {
            _A24--;
        } else if (_A24 < static_cast< u8 >(target)) {
            _A24++;
        }
        updateSimpleAlphaDL(_A24);
        _B18 = position;
        break;
    }
    }
    matrix.setTrans(position);
    PSMTXCopy(matrix.toMtxPtr(), _E0C.toMtxPtr());
    if (getCarrySensor()) {
        TPos3f carry;
        PSMTXConcat(_BC8.mMtx, MR::getJointMtx(getCarrySensor()->mHost, 0), carry.toMtxPtr());
        PSMTXConcat(_E0C.toMtxPtr(), carry.toMtxPtr(), _E0C.toMtxPtr());
        _9A0->calcType0(_E0C.toMtxPtr());
    }
    PSMTXConcat(_BC8.mMtx, offset.toMtxPtr(), offset.toMtxPtr());
    PSMTXConcat(matrix.toMtxPtr(), offset.toMtxPtr(), matrix.toMtxPtr());
    PSMTXConcat(j3dSys.mViewMtx, matrix.toMtxPtr(), j3dSys.mViewMtx);
    model->viewCalc3(3, nullptr);
    MR::loadViewMtx();
}

void MarioActor::calcViewWallShadowModel() {
    _A0C = 0;
    if ((_A08 & 2) || mBeeWallWalk) {
        return;
    }
    TVec3f direction;
    s32 length = 0;
    const AreaObj* area = MR::getAreaObj("ExtraWallCheckArea", _2A0);
    if (area) {
        MR::calcCubeAxisZ(area, &direction);
        length = MR::getAreaObjArg(area, 0);
        _20C = area;
        _210 = 0;
    } else {
        area = MR::getAreaObj("ExtraWallCheckCylinder", _2A0);
        if (area) {
            length = calcCylinderToCenter(area, &direction);
            _20C = area;
            _210 = 1;
        }
    }
    bool fading = false;
    if (!area || static_cast< f32 >(length) < 0.0f) {
        if (_208 >= 1000.0f) {
            return;
        }
        _208 += 10.0f;
        if (_208 >= 1000.0f) {
            _208 = 1000.0f;
        }
        switch (_210) {
        case 0:
            MR::calcCubeAxisZ(_20C, &direction);
            length = MR::getAreaObjArg(_20C, 0);
            break;
        case 1:
            length = calcCylinderToCenter(_20C, &direction);
            break;
        }
        fading = true;
    }
    Triangle triangle;
    TVec3f position;
    if (!MR::getFirstPolyOnLineToMap(&position, &triangle, _2A0, direction * static_cast< f32 >(length))) {
        return;
    }
    _1F0 = position;
    _1FC = *triangle.getNormal(0);
    if (!fading) {
        _208 = 0.9f * _208 + 0.1f * (position - _2A0).length();
    }
    updateRandomTexture(_208);
    TVec3f normal(direction);
    MR::normalizeOrZero(&normal);
    J3DModelX* model = getJ3DModel();
    J3DModelX* simple = getSimpleModel();
    PSMTXCopy(model->getBaseTRMtx(), simple->getBaseTRMtx());
    TPos3f& matrix = *reinterpret_cast< TPos3f* >(simple->getBaseTRMtx());
    TVec3f x;
    matrix.getXDir(x);
    MR::vecKillElement(x, normal, &x);
    TVec3f y;
    matrix.getYDir(y);
    MR::vecKillElement(y, normal, &y);
    TVec3f z;
    matrix.getZDir(z);
    MR::vecKillElement(z, normal, &z);
    matrix.setXDir(x);
    matrix.setYDir(y);
    matrix.setZDir(z);
    TVec3f horizontal;
    MR::vecKillElement(mPosition - position, getGravityVec(), &horizontal);
    TVec3f translation = mPosition + normal * (10.0f + horizontal.length());
    MR::setMtxTrans(matrix.toMtxPtr(), translation.x, translation.y, translation.z);
    PSMTXConcat(matrix.toMtxPtr(), _BC8.mMtx, matrix.toMtxPtr());
    simple->viewCalcRef(3, model);
    _A0C = 1;
}

void MarioActor::drawShadow() const {
    bool hidden = _482 || _481;
    if (hidden || !(_A08 & 1) || mPlayerMode == PlayerMode_Teresa) {
        return;
    }
    if (mBeeWallWalk && mMario->mMovementStates._1) {
        return;
    }
    if (mMario->isStatusActive(MarioStatus_FpView) && _1A1) {
        return;
    }
    if (_924 && selectNoShadow(_924)) {
        return;
    }
    if (mMario->mMovementStates._F && mMario->_544 > 1) {
        return;
    }
    J3DModelX* simple = getSimpleModel();
    TPos3f matrix;
    PSMTXScale(matrix.toMtxPtr(), mScale.x, mScale.y, mScale.z);
    PSMTXConcat(matrix.toMtxPtr(), _BC8.mMtx, matrix.toMtxPtr());
    PSMTXConcat(getJ3DModel()->getBaseTRMtx(), matrix.toMtxPtr(), simple->getBaseTRMtx());
    _214->calcView(simple, 2, getJ3DModel());
    if (mMario->mSinkTimer > 180) {
        return;
    }
    if (mMario->mSinkTimer > 128) {
        MR::hideJointAndChildren(simple, "Hip");
    }
    if (mMario->mSinkTimer > 150) {
        MR::hideJointAndChildren(simple, "ShoulderL");
        MR::hideJointAndChildren(simple, "ShoulderR");
    }
    if (_A58 || mMario->mSinkTimer > 120) {
        MR::hideJoint(simple, "HandL0");
        MR::hideJoint(simple, "HandR0");
    }
    _214->drawAndCaptureTex(simple, mPosition);
    _214->clearAlphaBuffer();
    if (mBeeWallWalk) {
        _214->_307 = 0;
    } else {
        _214->_307 = 1;
    }
    _214->draw();
    MR::showJointAndChildren(simple, "JointRoot");
}

void MarioActor::decideShadowMode() {
    if (mMario->mMovementStates._2 && !mMario->isNotReflectGlassGround() && !mMario->isStatusActive(MarioStatus_FpView) &&
        (mMario->_962 == 14 || mMario->_962 == 33)) {
        _A08 = 2;
        f32 distance = mMario->mVerticalSpeed / 500.0f;
        if (distance >= 0.75f) {
            distance = 0.75f;
        }
        u32 maximum = 64;
        u8 target = static_cast< f32 >(maximum) * (1.0f - distance);
        for (s32 i = 0; i < 4; i++) {
            if (_A09 < target) {
                _A09++;
            } else if (_A09 > target) {
                _A09--;
            }
        }
    } else if (_A09) {
        _A09--;
        if (_A09) {
            _A09--;
        }
        if (_A09) {
            _A09--;
        }
        if (_A09) {
            _A09--;
        }
        if (!_A09) {
            _A08 = 1;
        } else {
            _A08 = 2;
        }
    } else {
        _A08 = 1;
    }
    if (mMario->isSwimming() && MR::isInWater(mCamPos)) {
        if (_A25 || mCamDirZ.dot(-getGravityVector()) > 0.0f) {
            _A08 = 3;
        }
        if (_A24 || (mMario->mSwim->getWaterEdgeDist() > 0.0f && mMario->mSwim->getWaterEdgeDist() < 1000.0f)) {
            _A08 = 7;
        }
    }
    _214->setUpdateFlag();
}

void MarioActor::calcViewSilhouetteModel() {
    J3DModelX* model = getJ3DModel();
    J3DModelX* simple = getSimpleModel();
    TVec3f direction(mPosition - mCamPos);
    MR::normalizeOrZero(&direction);
    PSMTXCopy(model->getBaseTRMtx(), simple->getBaseTRMtx());
    TPos3f& matrix = *reinterpret_cast< TPos3f* >(simple->getBaseTRMtx());
    TVec3f x;
    matrix.getXDir(x);
    MR::vecKillElement(x, direction, &x);
    TVec3f y;
    matrix.getYDir(y);
    MR::vecKillElement(y, direction, &y);
    TVec3f z;
    matrix.getZDir(z);
    MR::vecKillElement(z, direction, &z);
    matrix.setXDir(x);
    matrix.setYDir(y);
    matrix.setZDir(z);
    TVec3f root;
    TVec3f translation;
    if (_EA4) {
        getRealPos("All_Root", &root);
        translation = root - direction * getConst().getTable()->mSilhouetteZoffset;
    } else {
        translation = mPosition;
    }
    MR::setMtxTrans(matrix.toMtxPtr(), translation.x, translation.y, translation.z);
    PSMTXConcat(matrix.toMtxPtr(), _BC8.mMtx, matrix.toMtxPtr());
    simple->viewCalcRef(1, model);
}

f32 MarioActor::calcCylinderToCenter(const AreaObj* pAreaObj, TVec3f* pVec) {
    TVec3f upVec;
    MR::calcCylinderUpVec(&upVec, pAreaObj);

    TVec3f centerPos;
    MR::calcCylinderCenterPos(&centerPos, pAreaObj);

    MR::vecKillElement(centerPos - _2A0, upVec, pVec);

    if (MR::normalizeOrZero(pVec)) {
        if (_208 >= 1000.0f) {
            return -1.0f;
        }

        _208 += 10.0f;

        if (_208 >= 1000.0f) {
            _208 = 1000.0f;
        }

        return -1.0f;
    }

    return MR::getAreaObjArg(pAreaObj, 0);
}

void MarioActor::calcViewBlurModel() {
    J3DModelX* model = getJ3DModel();
    if (model->_1E5) {
        if (_A6E == 1) {
            _A6E = 2;
        }
        model->_1E5 = 0;
    }
    if (MR::isDemoActive()) {
        if (_A6E) {
            _A6E = 5;
        }
        return;
    }
    if (_1C1) {
        return;
    }
    if (!_A6E) {
        return;
    }
    if (_A6E >= 3) {
        _A6E--;
        return;
    }
    if (_A6E == 2) {
        for (u32 i = 0; i < 8; i++) {
            for (u32 j = 0; j < getModelData()->getDrawMtxNum(); j++) {
                PSMTXCopy(model->getDrawMtx(j), _A70[i][j]);
                PSMTXCopy(model->getDrawMtx(j), _A90[i][j]);
            }
        }
        _B12 = 0;
        _A6E = 1;
    } else if (_37C % 3 == 0) {
        _B12 = (_B12 + 1) & 7;
        for (u32 j = 0; j < getModelData()->getDrawMtxNum(); j++) {
            PSMTXCopy(model->getDrawMtx(j), mBlurMatrices[1 - _B10][_B12][j]);
        }
    }
    _B10 = 1 - _B10;
    model->_1E4 = 1;
}

void MarioActor::calcViewFootPrint() {
    if (!_934 && mMario->mMovementStates._1 &&
        (mMario->mTargetWalkSpeedIndex || mMario->_1C.mIsUnderwater || mMario->mWalkSpeed > 0.1f || mMario->isPlayerModeHopper())) {
        if (!mMario->mSinkTimer && !mMario->mDrawStates._9 && !mMario->mDrawStates._A && mMario->_1C._13 && mMario->_1C._14 &&
            (mMario->checkCurrentFloorCodeSevere(13) || mMario->checkCurrentFloorCodeSevere(26))) {
            _B48->addPrint(mPosition, mMario->mFrontVec, mMario->_368, false);
        }
    }
    if (_B48->isValid(_37C) && MR::isInWater(*_B48->getPrintPos(_37C))) {
        _B48->invalidate(_37C);
    }
}

void MarioActor::drawSilhouette() const {
    bool hidden = _482 || _483 || _481;
    if (hidden) {
        return;
    }
    if (mMario->isStatusActive(MarioStatus_Sukekiyo)) {
        return;
    }
    if (mMario->isStatusActive(MarioStatus_Bury)) {
        if (!MR::isExistMapCollision(mCamPos, mPosition - mCamPos)) {
            return;
        }
        if (!MR::isExistMapCollision(mCamPos, _2AC - mCamPos)) {
            return;
        }
    }
    if (mMario->mSinkTimer || mPlayerMode == PlayerMode_Teresa || (mMario->isStatusActive(MarioStatus_FpView) && _1A1) || _EA4) {
        return;
    }
    if (_934) {
        return;
    }
    J3DModelX* model = getSimpleModel();
    model->setDrawView(1);
    model->mFlags.clear();
    model->mFlags._1C = true;
    model->directDraw(nullptr);
    model->mFlags._1C = false;
    GXSetAlphaUpdate(GX_TRUE);
    GXSetColorUpdate(GX_TRUE);
    GXSetDstAlpha(GX_TRUE, 0);
}
