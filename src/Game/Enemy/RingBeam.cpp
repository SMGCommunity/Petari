#include "Game/Enemy/RingBeam.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TUtil.hpp"

void RingBeam_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    f32 f3 = JGeometry::TUtil< f32 >::epsilon();
}

namespace NrvRingBeam {
    NEW_NERVE(RingBeamNrvSpread, RingBeam, Spread);
};  // namespace NrvRingBeam

RingBeamShadowDrawer::RingBeamShadowDrawer(const LiveActor* unk0) : ShadowVolumeDrawer("影描画[リングビーム]") {
    _1c = unk0;
    _20 = 0.0f;
}

void RingBeamShadowDrawer::drawShape() const {
    TPos3f MtxList[32];
    TPos3f baseMtxCopy;
    Mtx mtx;
    TVec3f temp;
    TVec3f temp2;
    TVec3f temp3;
    TVec3f listXDirScaledPos;
    TVec3f XDirScaled;
    TVec3f listXDirScaledNeg;
    TVec3f negYDirScaled;
    TVec3f XDir;
    TVec3f YDir;
    TVec3f Trans;
    TVec3f listXDir;
    TVec3f listYDir;
    f32 f1 = 0.2026834f;

    baseMtxCopy.setInline(_1c->getBaseMtx());
    baseMtxCopy.getXDir(XDir);
    baseMtxCopy.getYDir(YDir);
    baseMtxCopy.getTrans(Trans);
    PSMTXRotAxisRad(mtx, &YDir, f1);
    XDirScaled.scale(_20, XDir);
    f32 f2 = 0.0f;
    for (int i = 0; i < 32; i++) {
        temp.add(Trans, XDirScaled);
        MR::calcGravityVector(_1c, temp, &temp2, nullptr, 0);
        if (MR::normalizeOrZero(XDirScaled, &temp3)) {
            MtxList[i].setInline(baseMtxCopy);
        } else if (MR::isSameDirection(-temp2, temp3, 0.01f)) {
            MR::makeMtxUpSide(&MtxList[i], -temp2, -YDir);
        } else if (temp2.dot(YDir) < f2) {
            MR::makeMtxUpSide(&MtxList[i], -temp2, temp3);
        } else {
            MR::makeMtxUpSide(&MtxList[i], -temp2, -temp3);
        }
        // almost certainly an inline
        MtxList[i][0][3] = temp.x;
        MtxList[i][1][3] = temp.y;
        MtxList[i][2][3] = temp.z;

        PSMTXMultVec(mtx, &XDirScaled, &XDirScaled);
    }
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 64);
    for (int i = 0; i < 32; i++) {
        MtxList[i].getXDir(listXDir);
        MtxList[i].getTrans(Trans);
        listXDirScaledNeg.scale(-5.0f, listXDir);
        listXDirScaledPos.scale(5.0f, listXDir);
        temp.add(Trans, listXDirScaledNeg);
        temp2.add(Trans, listXDirScaledPos);
        GXPosition3f32(temp2.x, temp2.y, temp2.z);
        GXPosition3f32(temp.x, temp.y, temp.z);
    }
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 64);
    for (int i = 0; i < 32; i++) {
        MtxList[i].getXDir(listXDir);
        MtxList[i].getYDir(listYDir);
        MtxList[i].getTrans(Trans);

        listXDirScaledNeg.scale(-5.0f, listXDir);
        listXDirScaledPos.scale(5.0f, listXDir);
        negYDirScaled.scale(500.0f, -listYDir);
        temp.add(Trans, listXDirScaledNeg);
        temp2.add(Trans, listXDirScaledPos);
        temp.add(negYDirScaled);
        temp2.add(negYDirScaled);
        GXPosition3f32(temp.x, temp.y, temp.z);
        GXPosition3f32(temp2.x, temp2.y, temp2.z);
    }
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 64);
    for (int i = 0; i < 32; i++) {
        MtxList[i].getXDir(listXDir);
        MtxList[i].getYDir(listYDir);
        MtxList[i].getTrans(Trans);

        listXDirScaledNeg.scale(-5.0f, listXDir);
        negYDirScaled.scale(500.0f, -listYDir);
        temp.add(Trans, listXDirScaledNeg);
        temp2.add(Trans, listXDirScaledNeg);
        temp2.add(negYDirScaled);
        GXPosition3f32(temp.x, temp.y, temp.z);
        GXPosition3f32(temp2.x, temp2.y, temp2.z);
    }
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 64);
    for (int i = 0; i < 32; i++) {
        MtxList[i].getXDir(listXDir);
        MtxList[i].getYDir(listYDir);
        MtxList[i].getTrans(Trans);

        listXDirScaledPos.scale(5.0f, listXDir);
        negYDirScaled.scale(500.0f, -listYDir);
        temp.add(Trans, listXDirScaledPos);
        temp2.add(Trans, listXDirScaledPos);
        temp2.add(negYDirScaled);
        GXPosition3f32(temp2.x, temp2.y, temp2.z);
        GXPosition3f32(temp.x, temp.y, temp.z);
    }
}

void RingBeamShadowDrawer::loadModelDrawMtx() const {
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
};

bool RingBeamShadowDrawer::isDraw() const {
    return MR::isValidDraw(_1c);
}

RingBeam::RingBeam(const char* pName, LiveActor* arg1, bool arg2, bool arg3) : LiveActor(pName) {
    _8c = arg1;
    mSpeed = 20.0f;
    mLife = 100;
    _9c = arg2;
    _9d = arg3;
    _a0 = nullptr;
    _a4 = nullptr;
}

void RingBeam::setSpeed(f32 speed) {
    mSpeed = speed;
}

void RingBeam::setLife(s32 life) {
    mLife = life;
}

void RingBeam::init(const JMapInfoIter& rIter) {
    TPos3f mtx;
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("JumpBeamerBeam", nullptr, false);
    MR::connectToSceneIndirectEnemy(this);
    initHitSensor(1);
    MR::addHitSensorEye(this, "Beam", 8, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(1, nullptr, false);
    initSound(2, true);
    mSoundObject->setTrans(&_cc);
    MR::invalidateClipping(this);
    initNerve(&NrvRingBeam::RingBeamNrvSpread::sInstance);

    initPos(this);
    mtx.setInline(this->getBaseMtx());
    mtx.getZDir(_a8);
    mtx.getTrans(_c0);
    mtx.getTrans(_cc);
    if (_9d) {
        _a4 = new ModelObj("影モデル", "JumpBeamerBeamShadow", nullptr, MR::DrawBufferType_IndirectEnemy, MR::MovementType_Enemy,
                           MR::CalcAnimType_Enemy, false);
        _a4->initWithoutIter();
        _a4->mPosition.set(this->mPosition);
        _a4->mRotation.set(this->mRotation);
        _a4->mScale.set(this->mScale);
    } else {
        _a0 = new RingBeamShadowDrawer(this);
    }
    mBloomModel = MR::createModelObjBloomModel("リングビームブルーム", "JumpBeamerBeamBloom", getBaseMtx());
    MR::invalidateClipping(mBloomModel);
    makeActorDead();
}

void RingBeam::appear() {
    LiveActor::appear();
    mBloomModel->appear();
}

void RingBeam::kill() {
    setNerve(&NrvRingBeam::RingBeamNrvSpread::sInstance);
    initPos(_8c);
    setRadius(0.0f);
    LiveActor::kill();
    mBloomModel->kill();
}

void RingBeam::startBrk(const char* pchar) {
    MR::startBrk(this, pchar);
    if (_a4) {
        MR::startBrk(_a4, pchar);
    }
}

void RingBeam::setRadius(f32 radius) {
    TVec3f Ydir;
    TVec3f temp2;
    TVec3f temp6;

    if (radius >= 2000.0f) {
        radius = 2000.0f;
    }
    if (_a0) {
        _a0->_20 = radius;
    }
    getSensor("Beam")->mRadius = radius;
    char* str = "Spread";
    MR::startBckNoInterpole(this, str);
    MR::startBckNoInterpole(mBloomModel, str);
    radius = (radius / 2000.0f) * MR::getBckFrameMax(this);
    MR::setBckFrameAndStop(this, radius);
    MR::setBckFrameAndStop(mBloomModel, radius);
    if (_a4) {
        Triangle triangle;
        MR::extractMtxYDir(_8c->getBaseMtx(), &Ydir);
        Ydir.scale(1000.0f);

        MR::getFirstPolyOnLineToMap(&temp2, &triangle, _c0, _b4 * 500.0f);

        temp2 -= _b4 * 10.0f;
        MR::calcPerpendicFootToLine(&temp6, temp2, _8c->mPosition + Ydir, _8c->mPosition - Ydir);
        _a4->mPosition.set(temp6);
        MR::startBckNoInterpole(_a4, str);
        f32 length = (temp2 - temp6).length();
        if (0.0f <= length && length < 2000.0f) {
            MR::setBckFrameAndStop(_a4, (length / 2000.0f) * MR::getBckFrameMax(_a4));
        }
    }
}

void RingBeam::initPos(const LiveActor* actor) {
    TRot3f actorBaseMtxCopy;
    TVec3f baseMtxYdir;
    actorBaseMtxCopy.setInline(actor->getBaseMtx());
    actorBaseMtxCopy.getYDir(baseMtxYdir);
    mPosition.set(actor->mPosition + baseMtxYdir * 75.0f);
}

void RingBeam::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    TRot3f mtx;
    TVec3f temp;
    TVec3f temp2;
    TVec3f sensordiff = pReceiver->mPosition;
    sensordiff -= pSender->mPosition;
    MR::makeMtxTR((MtxPtr)&mtx, this);
    // this is 100% certainly an inline
    f32 x = mtx.mMtx[0][1];
    f32 y = mtx.mMtx[1][1];
    f32 z = mtx.mMtx[2][1];
    temp.set< f32 >(x, y, z);
    MR::vecKillElement(sensordiff, temp, &temp2);
    temp2.setLength(pSender->mRadius);

    TVec3f temp3 = temp2;
    temp3 -= sensordiff;
    f32 length = temp3.length();
    f32 receiverRadius = pReceiver->mRadius;

    if (!(length > 5.0f + receiverRadius) && MR::isSensorPlayerOrRide(pReceiver)) {
        MR::sendMsgEnemyAttackElectric(pReceiver, pSender);
    }
}

bool RingBeam::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

void RingBeam::exeSpread() {
    TPos3f mtx;

    if (MR::isFirstStep(this)) {
        if (!MR::isPowerStarGetDemoActive()) {
            MR::startSound(this, "SE_EM_JUMPRING_APPEAR");
        }
        char* str = "Spread";
        MR::startBckNoInterpole(this, str);
        MR::startBckNoInterpole(mBloomModel, str);
        startBrk("Erase");
        MR::setBrkFrameAndStop(this, 0.0f);
        if (_a4) {
            MR::setBrkFrameAndStop(_a4, 0.0f);
        }
        TVec3f temp;
        MR::extractMtxZDir(_8c->getBaseMtx(), &_a8);
        MR::extractMtxYDir(_8c->getBaseMtx(), &temp);
        _c0.set(_8c->mPosition + temp * 75.0f);
        MR::calcGravityVector(this, _c0, &_b4, nullptr, 0);
    }

    TVec3f ec;
    if (_9c) {
        MR::calcGravityVector(this, _c0 + _a8 * mSpeed, &_b4, nullptr, 0);
        MR::makeMtxUpFront(&mtx, -_b4, _a8);
        mtx.getZDir(_a8);
        TVec3f temp7;
        MR::extractMtxYDir(_8c->getBaseMtx(), &temp7);
        MR::calcPerpendicFootToLine(&ec, _c0, _8c->mPosition + temp7, _8c->mPosition - temp7);
        mPosition = ec.copy();
    } else {
        ec = mPosition;
    }

    _c0 += _a8 * mSpeed;
    setRadius((_c0 - ec).length());

    if (!MR::isPowerStarGetDemoActive()) {
        TVec3f temp12 = MR::getCamPos() - ec;
        if (!MR::isNearZero(temp12)) {
            f32 radius;
            f32 float2;
            TVec3f temp13;
            MR::extractMtxYDir(_8c->getBaseMtx(), &temp13);
            TVec3f temp15 = temp12 - temp13 * (temp12.dot(temp13));
            MR::normalizeOrZero(&temp15);
            MR::normalizeOrZero(&temp12);
            float2 = 1.0f - __fabsf(temp13.dot(temp12));
            radius = getSensor("Beam")->mRadius;
            _cc = ec + temp15 * radius * float2;
        } else {
            _cc = ec;
        }
        MR::startLevelSound(this, "SE_EM_LV_RINGBEAM_MOVE");
    }
    if (MR::isStep(this, mLife - (s32)MR::getBrkFrameMax(this))) {
        startBrk("Erase");
    }
    if (MR::isGreaterEqualStep(this, mLife)) {
        kill();
    }
}
