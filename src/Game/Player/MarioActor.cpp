#include "Game/Player/MarioActor.hpp"
#include "Game/Animation/XanimeCore.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/Enemy/KarikariDirector.hpp"
#include "Game/Gravity/GravityInfo.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Map/WaterInfo.hpp"
#include "Game/Player/J3DModelX.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioEffect.hpp"
#include "Game/Player/MarioHolder.hpp"
#include "Game/Player/MarioMessenger.hpp"
#include "Game/Player/MarioNullBck.hpp"
#include "Game/Player/MarioParts.hpp"
#include "Game/Player/MarioShadow.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Player/ModelHolder.hpp"
#include "Game/Player/RushEndInfo.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/FixedPosition.hpp"
#include "Game/Util/FootPrint.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTVideo.hpp>

bool gIsLuigi;
static f32 BASE_ROTATION = 0.0f;

Triangle& Triangle::operator=(const Triangle& rOther) {
    mParts = rOther.mParts;
    mIdx = rOther.mIdx;
    mSensor = rOther.mSensor;
    mNormals[0] = rOther.mNormals[0];
    mNormals[1] = rOther.mNormals[1];
    mNormals[2] = rOther.mNormals[2];
    mNormals[3] = rOther.mNormals[3];
    mPos[0] = rOther.mPos[0];
    mPos[1] = rOther.mPos[1];
    mPos[2] = rOther.mPos[2];

    return *this;
}

MarioActor::MarioActor(const char* pName) : LiveActor(pName), _1B0(0xFFFFFFFF) {
    initMember();
    mMario = new Mario(this);
    _930 = 0;
    _468 = 0;
    mMaxHealth = 3;
    mHealth = 3;
    mWaterLife = 8;

    if (MR::isPlayerLuigi()) {
        mMaxHealth = 3;
        mHealth = 3;
    }

    if (MR::isGalaxyDarkCometAppearInCurrentStage()) {
        mMaxHealth = 1;
        mHealth = 1;
    }
    init2D();
    _989 = 0;
    _41C = 0;
    _420 = 0;
    _37C = 0;
    _92C = 0;
    _388 = 0;
    _38C = 0;
    _390 = 0;
    _394 = 0;
    _398 = 0;
    _3A4 = 0;
    _3A8 = 0;
    _3AA = 0;
    _944 = 0;
    _946 = 0;
    _BC4 = 0;
    _948 = 0;
    _94C = 0;
    _94E = 0;
    _94A = 0;
    _47C = 0;
    _3DC = 0;
    _3D8 = 0;
    _3DA = 0;
    _3AC = 0;
    _B94 = 0;
    _378 = 0;
    _6D4 = 0.0f;
    mSuperKinokoCollected = false;
    mPowerupCollected = false;
    mTransforming = false;
    _950 = 0;
    _951 = 0;
    _974 = 0;
    _39C = 0;
    _3B0 = 1.0f;

    _3B4.zero();

    _3C1 = false;
    _211 = 0;
    _46C = 0;
    _470 = 0;
    _474 = 0;
    _924 = nullptr;
    _928 = 0;
    _480 = 0;
    _481 = 0;
    _482 = 0;
    _483 = 0;

    _484.zero();

    _B92 = 0;
    mPlayerMode = 0;
    _3D6 = 0;
    _3D2 = 1;
    _3DF = 0;
    _3DE = 0;
    _EEB = 1;
    _9D4 = 0;

    _9CC = 0.0f;
    _9D0 = 0.0f;

    _9D8.zero();

    mAlphaEnable = false;
    _9F2 = false;
    _EA4 = false;
    _EA5 = false;
    _FCC = false;
    _FCD = false;

    PSMTXIdentity((MtxPtr)&_EA8);
    PSMTXIdentity((MtxPtr)&_3EC);

    _1F0.zero();
    _1FC.zero();

    mBlackHole = nullptr;
    _208 = 1000.0f;

    mBlackHolePosition.zero();
    mBlackHoleRotateAxis.zero();
    mPosRelativeToBlackHole.zero();

    for (int i = 0; i < 6; i++) {
        mModels[i] = nullptr;
    }

    _A61 = false;

    _A64 = 0xff;

    _A68 = 1.0f;

    _A6C = 0;
    _B48 = nullptr;
    _934 = false;
    _7E2 = 0;
    _EF6 = 0;
    _424 = 0;
    _4A4 = nullptr;
    _6D0 = 0;
    _3A0 = 0;
    _EFC = 0;
    _EF8 = 0;
    mBlendMtxTimer = 0;

    _EA0 = 1.0f;

    _3C4.zero();

    _3D0 = 0;
    _7DE = 0;
    _3C0 = false;
    _B90 = false;
    _B91 = false;
    _39D = 0;
    _B96 = 0;
    _EF0 = 0;
    _EF2 = 0;
    _EF4 = 0x78;
    _1AA = 0;
    _1AC = 0.0f;

    _1B0.set(0xFF, 0xFF, 0xFF, 0);

    _1B5 = false;
    _988 = 0;

    _F10 = 2;
    _F12 = 2;

    _F14 = 3;
    _F16 = 3;

    _F18 = 0xb;

    _F1A = 3;

    _F0E = 7;

    resetPadSwing();

    _F0D = 0;

    _F1C = 0x3C;
    _F1E = 0x3C;

    _1E1 = 0;
    _F24 = 0;
    _F28 = 0;

    PSMTXIdentity((MtxPtr)&_C2C);
    PSMTXIdentity((MtxPtr)&_C5C);
    PSMTXIdentity((MtxPtr)&_D1C);
    PSMTXIdentity((MtxPtr)&_D4C);
    PSMTXIdentity((MtxPtr)&_D7C);

    _1B4 = 0;
    _1C3 = false;
    _1E0 = false;
}

MarioActor::~MarioActor() {
    delete mMario;
}

void MarioActor::init(const JMapInfoIter& rInfo) {
    s32 initialAnimation = -1;
    if (!MR::isValidInfo(rInfo)) {
        return;
    }
    TVec3f position, rotation, scale;

    MR::getJMapInfoTrans(rInfo, &position);
    MR::getJMapInfoRotate(rInfo, &rotation);
    MR::getJMapInfoScale(rInfo, &scale);

    rotation.x = BASE_ROTATION + (f32)fmod(rotation.x - BASE_ROTATION + 360.0f, 360.0);
    rotation.y = BASE_ROTATION + (f32)fmod(rotation.y - BASE_ROTATION + 360.0f, 360.0);
    rotation.z = BASE_ROTATION + (f32)fmod(rotation.z - BASE_ROTATION + 360.0f, 360.0);

    if (MR::isExistJMapArg(rInfo)) {
        MR::getJMapInfoArg0NoInit(rInfo, &initialAnimation);
    }
    init2(position, rotation, initialAnimation);
}

struct DUMMY {
    u8 _0[0x30];
};

void MarioActor::init2(const TVec3f& a, const TVec3f& b, s32 initialAnimation) {
    _8C = 1;
    gIsLuigi = false;
    if (MR::isPlayerLuigi()) {
        gIsLuigi = true;
    }
    mPosition.set(a);
    mRotation.set(b);
    mScale.set(TVec3f(1.0f, 1.0f, 1.0f));
    mMario->setHeadAndFrontVecFromRotate(mRotation);
    mMario->_290 = mMario->mSideVec;
    updateBaseScaleMtx();
    _A18 = mRotation;
    initDrawAndModel();

    _C28 = new Mtx[MR::getJointNum(this)];

    MR::connectToScene(this, MR::MovementType_Player, MR::CalcAnimType_Player, MR::DrawBufferType_Player, MR::DrawType_Player);
    MR::initLightCtrlForPlayer(this);
    mMarioAnim = new MarioAnimator(this);
    mMarioEffect = new MarioEffect(this);
    _214 = new CollisionShadow(100.0f, 360.0f);
    mConst = new MarioConst();
    if (gIsLuigi) {
        mConst->changeTable(1);
    }
    mMario->initAfterConst();
    _36C = new GravityInfo();
    _374 = 0.0f;
    initNerve(&NrvMarioActor::MarioActorNrvWait::sInstance);
    _FB4 = 0;
    _FB8 = 0;
    initActionMatrix();
    initBinder(60.0f, 0.0f, 8);
    _2C4.x = 0.0f;
    _2C4.y = 70.0f;
    _2C4.z = 0.0f;
    mBinder->_1EC._0 = false;
    MR::setBinderOffsetVec(this, &_2C4, false);

    mBinder->setTriangleFilter(TriangleFilterDelegator< MarioActor >::allocateDelegator(this, &MarioActor::binderFilter));

    mBinder->_1EC._3 = true;
    initEffect();
    MR::invalidateClipping(this);

    _240.set(0.0f, -1.0f, 0.0f);

    _24C = _240;
    _334 = 0;
    _336 = 0;
    _338 = 0;

    _264.zero();
    _270 = mPosition;
    calcCenterPos();
    initSound(0x10, 0);
    addToSoundObjHolder();
    initParts();
    initMorphStringTable();
    MR::declareGlobalEventCameraAbyss("奈落カメラ");
    MR::declareBlackHoleCamera("ブラックホール");
    MR::declareGlobalEventCameraDead("昇天カメラ", 0.35f, 0x78, 0x3C);
    MR::declareGlobalEventCamera("水中フォロー");
    MR::declareGlobalEventCamera("水中プラネット");
    MR::declareGlobalEventCamera("水上フォロー");
    MR::declareGlobalEventCamera("引き戻し");
    MR::declareEventCameraProgrammable("変身初出カメラ");
    _2B8 = mPosition;
    _33C = mPosition;
    _348.zero();
    _354.zero();
    setupSensors();
    MR::getMarioHolder()->setMarioActor(this);
    _1BC = new MarioMessenger(getSensor("dummy"));
    _300 = mMario->mHeadVec;
    _2D0 = _300;
    _330 = 0;
    _332 = 0;
    MR::setGameCameraTargetToPlayer();
    _A0C = 0;
    _B48 = new FootPrint("足跡", 0x100, -1);
    _B48->setTexture(MR::getTexFromArc("Footprint.bti", this));
    switch (initialAnimation) {
    case 1:
        mMario->changeAnimation("基本", (const char*)nullptr);
        break;
    case 2:
        mMario->changeAnimationNonStop("ウォークイン");
        break;
    default:
        mMario->changeAnimation("ステージインA", (const char*)nullptr);
        break;
    }
    updateTransForCamera();
    _F44 = 1;
    _984 = 0.0f;
    _978.zero();
    _27C.zero();
    _288.zero();
    _498 = new FixedPosition(this, "HandR", TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
    _49C = new FixedPosition(this, "HandR", TVec3f(1.8f, 52.51f, 39.58f), TVec3f(76.33f, 15.69f, 88.99f));
    _494 = 0;
    _4B0 = 35.0f;
    _4B4 = 60.0f;
    _4B8.set(0.0f, 1.0f, 0.0f);
    _4C4 = -_4B8;
    _482 = true;
    appear();
    _482 = false;  // do we change this to control appearances?
    _1C6 = 0;
    _1C8 = 0.0f;
    _1CC = 0.0f;
    _1D0 = 0;
    _1D1 = 0;
    _A24 = 0;
    _A25 = 0;
    _1D8 = new (0x20) FBO[MR::getFrameBufferWidth()];
    _1DC = new (0x20) FBO[MR::getFrameBufferWidth()];
    _1E4 = 0.0f;
    _1E8 = 0;
    _1EC = 0.0f;
    _F3CVec = new TVec3f[30];
    _F40 = 0;
    _F42 = 1;
    for (int i = 0; i < 30; i++) {
        _F3CVec[i].set(1.0f, 0.0f, 0.0f);
    }
    _8C = 0;  // is this to indicate that we are in the process of initialization?
}

void MarioActor::initAfterPlacement() {
    updateGravityVec(true, true);
    mMario->mAirGravityVec = _240;
    mMario->mHeadVec = -_240;
    mMario->_1FC = -_240;
    _300 = mMario->mHeadVec;
    _2D0 = _300;
    _2C4 = _240 * -70.0f;
    calcCenterPos();
    MR::updateHitSensorsAll(this);
    _360 = getGravityVector();
    _1C0 = true;
    _9F4 = getGravityVector();
    updateCameraInfo();
    calcBaseFrontVec(-_240);
}

void MarioActor::initAfterOpeningDemo() {
    mMario->changeAnimationNonStop("ウォークイン");
    _37C = 0;
}

void MarioActor::calcBaseFrontVec(const TVec3f& rVec) {
    TVec3f cross, j(0.0f, 1.0f, 0.0f);
    f32 y = j.dot(rVec);
    if (y < -0.99f) {
        _258.set(0.0f, 0.0f, 1.0f);
    } else {
        f32 cosine;
        if (MR::makeAxisAndCosignVecToVec(&cross, &cosine, rVec, j)) {
            TVec3f k(0.0f, 0.0f, 1.0f);
            Mtx transformation;
            PSMTXRotAxisRad(transformation, &cross, -JMAAcosRadian(cosine));
            PSMTXMultVecSR(transformation, &k, &_258);
            MR::normalize(&_258);
        } else {
            _258.set(0.0f, 0.0f, 1.0f);
        }
    }
}

void MarioActor::playSound(const char* pName, s32 duration) {
    mMario->playSound(pName, duration);
}

void MarioActor::changeAnimation(const char* pName, const char* pNewName) {
    mMario->changeAnimation(pName, pNewName);
}

void MarioActor::changeAnimationNonStop(const char* pName) {
    mMario->changeAnimationNonStop(pName);
}

void MarioActor::changeAnimationUpper(const char* pName) {
    if (!mMario->_71C && isAnimationRun("基本")) {
        mMario->changeAnimation(pName, static_cast< const char* >(nullptr));
        return;
    }

    mMario->changeAnimationUpper(pName, nullptr);
}

void MarioActor::stopAnimation(const char* pName) {
    mMario->stopAnimation(pName, (const char*)nullptr);
}

bool MarioActor::isAnimationRun(const char* pName) const {
    return mMario->isAnimationRun(pName);
}

void MarioActor::changeNullAnimation(const char* pName, s8 num) {
    mNullAnimation->appear();
    MR::startBck(mNullAnimation, pName, nullptr);
    _B92 = num;
}

void MarioActor::clearNullAnimation(s8 num) {
    _B92 = num;
    mNullAnimation->kill();
}

bool MarioActor::isStopNullAnimation() const {
    if (!MR::isBckStopped(mNullAnimation)) {
        return MR::isDead(mNullAnimation);
    }
    return true;
}

void MarioActor::changeGameOverAnimation() {
    int animation = 0;
    if (mMario->isStatusActive(MarioStatus_FpView)) {
        mMario->closeStatus(nullptr);
    }

    if (mMario->isAnimationRun("前方小ダメージ")) {
        animation = 0;
    }
    if (mMario->isAnimationRun("後方小ダメージ")) {
        animation = 0;
    }
    if (mMario->isAnimationRun("ファイアラン前兆")) {
        animation = 0;
    }
    if (mMario->isAnimationRun("炎のランナー")) {
        animation = 0;
    }
    if (mMario->isAnimationRun("電気ダメージ")) {
        animation = 1;
    }
    if (mMario->isAnimationRun("電気ダメージ終了")) {
        animation = 1;
    }
    if (mMario->isAnimationRun("炎ダメージ")) {
        animation = 2;
    }
    if (mMario->isAnimationRun("ファイアダンス")) {
        animation = 2;
    }
    if (mMario->isAnimationRun("中ダメージ")) {
        animation = 3;
    }
    if (mMario->isAnimationRun("中ダメージ空中")) {
        animation = 3;
    }
    if (mMario->isAnimationRun("中ダメージ着地")) {
        animation = 3;
    }
    if (mMario->isAnimationRun("中後ダメージ")) {
        animation = 4;
    }
    if (mMario->isAnimationRun("中後ダメージ空中")) {
        animation = 4;
    }
    if (mMario->isAnimationRun("中後ダメージ着地")) {
        animation = 4;
    }
    if (mMario->isAnimationRun("落下")) {
        animation = 5;
    }
    if (mMario->isAnimationRun("空中ふんばり")) {
        animation = 5;
    }
    if (mMario->isAnimationRun("つぶれ")) {
        animation = 6;
    }
    if (mMario->isAnimationRun("つぶれ復帰")) {
        animation = 6;
    }

    if (mMario->isAnimationRun("氷結") || mMario->isStatusActive(MarioStatus_Freeze)) {
        animation = -1;
    }

    if (mMario->isSwimming()) {
        animation = 7;
    }
    if (isNerve(&NrvMarioActor::MarioActorNrvGameOverSink::sInstance)) {
        animation = 8;
    }
    if (MR::getKarikariClingNum() && mMario->getMovementStates()._1) {
        animation = 4;
    }
    if (_39D == 2) {
        animation = 9;
    }
    if (_39D == 1) {
        animation = 10;
    }

    switch (animation) {
    case 0:
        mMario->changeAnimationNonStop("座りダウン");
        break;
    case 1:
        mMario->changeAnimationNonStop("感電ダウン");
        break;
    case 2:
        mMario->changeAnimationNonStop("炎ダウン");
        break;
    case 3:
        mMario->changeAnimationNonStop("仰向けダウン");
        break;
    case 4:
        mMario->changeAnimationNonStop("俯せダウン");
        break;
    case 5:
        if (mMario->getMovementStates()._1) {
            mMario->changeAnimationNonStop("座りダウン");
        } else {
            mMario->changeAnimationNonStop("奈落ダウン");
        }
        break;
    case 6:
        mMario->changeAnimationNonStop("つぶれダウン");
        break;
    case 7:
        mMario->changeAnimationNonStop("水泳ダウン");
        break;
    case 8:
        mMario->changeAnimationNonStop("埋まりダウン");
        break;
    case 9:
        mMario->changeAnimationNonStop("レース負け");
        break;
    case 10:
        mMario->changeAnimationNonStop("水中レース負け");
        break;
    }
    _B90 = true;
    stopEffect("無敵中");
    _A6E = false;
}

XjointTransform* MarioActor::getJointCtrl(const char* pName) const {
    XanimeCore* pCore = mMarioAnim->mXanimePlayer->mCore;
    return pCore->getJointTransform(MR::getJointIndex(this, pName));
}

void MarioActor::updateRotationInfo() {
    TRot3f stack_44;
    PSMTXConcat(getBaseMtx(), _E3C.toMtxPtr(), stack_44.toMtxPtr());
    MR::makeRTFromMtxPtr(nullptr, &mRotation, stack_44.toMtxPtr(), true);
    if (mRotation.z > 90.0f && mRotation.x > 90.0f) {
        f32 diff = 180.0f - mRotation.y;
        mRotation.z = 0.0f;
        mRotation.y = diff;
        mRotation.x = 0.0f;
    }
    stack_44.getEuler(_318);
    _318.mult(57.2957763672f);
    stack_44.getEuler(_324);
    _324.mult(57.2957763672f);
    if (MR::isSameDirection(_240, mMario->mFrontVec, .01f)) {
        _A18 = mRotation;
    } else {
        TPos3f stack_14;
        TVec3f stack_8 = -_240;
        MR::makeMtxUpFront(&stack_14, stack_8, mMario->mFrontVec);
        MR::makeRTFromMtxPtr(nullptr, &_A18, stack_14.toMtxPtr(), true);
        if (_A18.z > 90.0f && _A18.x > 90.0f) {
            f32 diff = 180.0f - _A18.y;
            _A18.z = 0.0f;
            _A18.y = diff;
            _A18.x = 0.0f;
        }
    }
}

void MarioActor::exeWait() {
    if (_FB8 != 0) {
        _FB8--;
        if (_FB8 == 0) {
            setNerve(_FB4);
            _FB4 = nullptr;
        }
    }
}

void MarioActor::movement() {
    // FIXME: wrong stack
    _46C = nullptr;
    _378++;
    _1E1 = 0;
    PSMTXCopy(_AE0.toMtxPtr(), _AB0.toMtxPtr());
    PSMTXCopy(MR::getCameraViewMtx(), _AE0.toMtxPtr());
    updateCameraInfo();
    _4A8 = 0;
    _4AC = HALF_PI / 2.0f;
    LiveActor::movement();
    TVec3f stack_134(mPosition);
    stack_134 -= _294;
    TVec3f stack_128(stack_134);
    stack_128 -= mVelocity;
    _27C = stack_134;
    TVec3f stack_11C(_288);
    _288 = stack_128;
    if (MR::isOppositeDirection(_288, stack_11C, 0.01f)) {
        f32 mag_288 = _288.length();
        f32 magStack_11C = stack_11C.length();
        if (!MR::isNearZero(mag_288) && !MR::isNearZero(magStack_11C) && MR::isNearZero(mag_288 - magStack_11C, 1.0f)) {
            mPosition -= _288 * 0.5f;
        }
    }
    if (stack_128.length() > 0.1f) {
        if (!(getMovementStates()._A)) {
            if (!MR::isNearZero(mVelocity)) {
                TVec3f stack_110(_294);
                stack_110 -= _270;
                f32 diffMag = stack_110.length();
                f32 vMag = mVelocity.length();
                if (stack_128.length() > 2.0f * (diffMag + vMag)) {
                    mMario->stopWalk();
                }
            }
        }
        if (getMovementStates()._23 && mVelocity.length() < stack_134.length()) {
            if (stack_134.dot(getGravityVec()) < -0.0f) {
                TVec3f stack_110;
                MR::vecKillElement(mVelocity, getGravityVec(), &stack_110);
                if (MR::isNearZero(stack_110)) {
                    MR::vecKillElement(stack_134, getGravityVec(), &stack_110);
                }
                stack_110.setLength(stack_134.length());  // needs to be inlined
                mMario->push(stack_110);
                if (mMario->_3BC <= 2) {
                    f32 scale = stack_128.length();
                    if (scale > 10.0f) {
                        scale = 10.0f;
                    }
                    mMario->mJumpVec += -getGravityVec() * scale;
                }
            }
        } else if (getMovementStates()._23) {
            TVec3f stack_104(mMario->_8F8);
            MR::normalizeOrZero(&stack_104);
            TVec3f stack_F8;
            f32 elementA = MR::vecKillElement(stack_134, stack_104, &stack_F8);
            f32 elementB = MR::vecKillElement(mVelocity, stack_104, &stack_F8);
            if (mVelocity.length() > 20.0f && elementA < elementB * 0.5f) {
                if (mMario->isAnimationRun("坂すべり下向きあおむけ")) {
                    mMario->push(mMario->mFrontVec * 5.0f);
                } else if (mMario->isAnimationRun("坂すべり上向きうつぶせ")) {
                    mMario->push(mMario->mFrontVec * -5.0f);
                }
                mMario->mDrawStates._2 = true;
            }
        }
        if (getMovementStates().jumping && !mAlphaEnable) {
            if (stack_128.dot(getGravityVec()) < -40.0f) {
                TVec3f stack_EC(mPosition - getGravityVec() * 100.0f);
                TVec3f stack_E0;
                Triangle* pTmp = mMario->getTmpPolygon();

                if (MR::getFirstPolyOnLineToMap(&stack_E0, pTmp, stack_EC, getGravityVec() * 200.f)) {
                    TVec3f stack_D4;
                    if (MR::vecKillElement(stack_E0 - (mPosition), getGravityVec(), &stack_D4) < -5.0f && pTmp->mParts && !pTmp->mParts->_D4 &&
                        getMovementStates()._3E != 1) {
                        mPosition = stack_E0;
                        mMario->mPosition = mPosition;
                        mMario->stopJump();
                        mMario->changeAnimation("基本", "落下");
                        mMario->updateGroundInfo();
                    }
                }
            } else if (stack_128.dot(getGravityVec()) > 40.0f) {
                if (mMario->_4C8->isValid()) {
                    if (MR::isSensorPressObj(mMario->_4C8->mSensor)) {
                        mMario->mDrawStates._1E = true;
                    }
                } else {
                    const u32 stop = mBinder->mPlaneNum;
                    for (int i = 0; i < stop; i++) {
                        if (MR::isSensorPressObj(mBinder->getPlane(i)->mSensor)) {
                            mMario->mDrawStates._1E = true;
                        }
                    }
                }
                if ((mMario->mShadowPos - mPosition).dot(getGravityVec()) < 0.0f) {
                    bool eject = true;
                    CollisionParts* parts = mMario->_45C->mParts;
                    if (parts && !mMario->_45C->mParts->_D4) {
                        TVec3f stack_C8, stack_BC, stack_B0;
                        PSMTXMultVec(parts->mInvBaseMatrix.toMtxPtr(), &mMario->mShadowPos, &stack_C8);
                        PSMTXMultVec(parts->mPrevBaseMatrix.toMtxPtr(), &stack_C8, &stack_BC);
                        stack_B0 = mMario->mShadowPos - stack_BC;
                        if (stack_B0.dot(stack_128) > 0.0f) {
                            eject = false;
                        }
                    }
                    if (eject) {
                        mPosition = mMario->mShadowPos;
                        mMario->mJumpVec.zero();
                        mMario->_148.zero();
                        if (!mMario->_5FC && (getDrawStates()._1E || getPrevDrawStates()._1E)) {
                            TVec3f stack_A4(stack_128);
                            MR::normalizeOrZero(&stack_A4);
                            _3B4 = stack_A4;
                            setPress(0, 0);
                            _3B0 = 0.1f;
                        }
                    }
                }
            }
        } else if (mMario->getMovementStates()._1) {
            const u32 stop = mBinder->mPlaneNum;
            bool eject = false;
            for (u32 i = 0; i < stop; i++) {
                const Triangle* plane = mBinder->getPlane(i);
                if (!MR::isSensorPressObj(plane->mSensor)) {
                    continue;
                }
                if (mMario->_368.dot(*plane->getNormal(0)) > 0.0f) {
                    if (mMario->_72C < 200.0f) {
                        eject = true;
                    } else if (plane->getNormal(0)->dot(stack_134) < 0.0f) {
                        eject = true;
                    } else {
                        eject = false;
                    }
                } else if (plane->getNormal(0)->dot(stack_134) < 0.0f) {
                    eject = true;
                } else if (mMario->_5FC) {
                    if (!MR::isWallCodeNoAction(plane) && !mMario->isOnimasuBinderPressSkip()) {
                        _3B4 = mMario->_368;
                        mMario->_10._1D = false;
                        *mMario->_4C8 = *plane;
                        setPress(2, 0);
                        _3B0 = 0.1f;
                    }
                    eject = true;
                }
            }
            if (eject) {
                TVec3f stack_98;
                f32 element = MR::vecKillElement(stack_134, mMario->_368, &stack_98);
                mPosition -= mMario->_368 * element;
            }
        }
    }
    if (mMario->mDrawStates._1A) {
        mMario->mDrawStates._1A = false;
        mMarioAnim->MarioAnimator::update();
    }
    mMario->recordRelativePosition();
    updateTransForCamera();
    calcAnimInMovement();
    _935 = false;
    mMario->_2D0 = 0.0f;
    _F3CVec[_F40] = mMario->mFrontVec;
    _F40 = (u16)(_F40 + 1) % _F42;
}

void MarioActor::control() {
    if (mSuperKinokoCollected) {
        if (MR::tryStartDemoWithoutCinemaFrame(this, "マリオスーパー化")) {  // 6-up camera
            mSuperKinokoCollected = false;
            changeMaxLife(6);
            MR::stopAnimFrame(this);
            MR::requestPowerUpHPMeter();
            mMario->startPadVib("マリオ[変身]");
            MR::startSystemSE("SE_SY_SUPER_KINOKO_GET");
            _3DA = 0x5a;
        }
    } else if (mPowerupCollected) {
        if (MR::tryStartDemoWithoutCinemaFrame(this, "マリオ変身")) {
            mPowerupCollected = false;
            mTransforming = true;
            if (mPlayerMode == 6) {
                MR::requestMovementOn(_9A4);
                changeTeresaAnimation("change", -1);
            }
            _3D8 = 0x40;
            MR::stopAnimFrame(this);
            playEffect("変身");
            mMario->startPadVib("マリオ[変身]");
        }
    }
    control2();
    _294 = mPosition;
}

void MarioActor::control2() {
    if ((_3D8 && mPlayerMode) || _3DA) {
        mMario->_350.zero();
        mMario->_35C.zero();
        _7DC = 0;
        _930 = 0;
        mVelocity.zero();
        _264.zero();
        _270 = mPosition;
        if (getMovementStates()._1 && !MR::isSameMtx(mMario->_45C->getBaseMtx()->toMtxPtr(), mMario->_45C->getPrevBaseMtx()->toMtxPtr())) {
            mMario->mPosition = mPosition;
            mMario->checkEnforceMove();
            mPosition = mMario->mPosition;
        }
    } else {
        if (_BC4) {
            _BC4--;
        }
        _951 = 0;
        controlMain();
        if (!_EA4) {
            if (!_F44) {
                MR::offBind(this);
            } else {
                MR::onBind(this);
            }
        }
        _978 = mVelocity;
        if (getDrawStates()._B) {
            mBinder->_1EC._1 = false;
        } else {
            mBinder->_1EC._1 = true;
        }
        if (mMario->isDamaging()) {
            _424 = 0;
        }
    }
}

void MarioActor::controlMain() {
    if (_3C0) {
        _7DC = 0;
        _930 = 0;
    }
    updateActionTrigger();
    updateControllerSwing();
    updateControllerSwingLeft();
    update2D();
    if (_A61) {
        _A6C++;
    }
    if (!isEnableMoveMario()) {
        _7DC = 0;
        _930 = 0;
        careMovingObject();
    } else {
        mVelocity.zero();
        updateBehavior();
        updateRotationInfo();
        updateTakingPosition();
        updateSearchLight();
        updateThrowing();
        updateBeeWingAnimation();
        updateFairyStar();
        _930 = 0;
    }
}

void MarioActor::updateBehavior() {
    updateLife();
    updatePlayerMode();
    _37C++;
    if (_EF6) {
        _EF6--;
    }
    if (_3A8) {
        _3A8--;
    }
    if (_3AC) {
        _3AC--;
    }
    TVec3f vec = mPosition;
    vec -= _270;
    _264 = vec;
    _270 = mPosition;
    updateBindRatio();
    updateEffect();
    if (_B94 && !--_B94) {
        mMario->stopAnimationUpper("ハンマー投げ回転中", nullptr);
        mMario->stopAnimation("ハンマー投げ回転中", (const char*)nullptr);
    }
    updatePunching();
    if (!doPressing() && !doStun() && !doRush()) {
        updateGravityVec(false, false);
        if (!tryJumpRush()) {
            checkPriorRushTarget();
            if (!tryThrow() && !tryStandardRush()) {
                if (getMovementStates()._1) {
                    initForJump();
                }
                updateSwingTimer();
                updateSwingAction();
                updateThrowVector();
                lockOnDPD();
                if (!checkClapCatchStart()) {
                    _7DC = 0;
                    mMario->Mario::update();
                    updateForCamera();
                    updateTornado();
                    tryCoinPull();
                    mMario->mDrawStates._1A = true;
                }
            }
        }
    }
}

void MarioActor::updateBindRatio() {
    // FIXME: wrong stack
    if (!_934 && !MR::isNearZero(_978 - _264)) {
        f32 mag = _978.length();
        TVec3f stack_38(_978);
        stack_38 -= _264;
        if (mag / stack_38.length() < 2.0f) {
            _984 += 0.1f;
        } else {
            _984 -= 0.01f;
        }
    } else if (!MR::isNearZero(_978)) {
        _984 -= 0.01f;
    }
    _984 = MR::clamp(_984, 0.0f, 1.0f);
}

bool MarioActor::isInPunchTimerRange() const {
    bool ret = false;
    if (_3E6) {
        ret = MR::isInRange(_945, 6.0f, 20.0f);
    }
    return ret;
}

void MarioActor::updatePunching() {
    if (_944) {
        bool canSpinring = true;
        _944--;
        _945++;
        if (!_946 && !getMovementStates()._2B) {
            canSpinring = false;
        }
        if (canSpinring && !mMario->isSwimming() && !_944 && selectAction("スピン回復エフェクト") == true) {
            playEffect("スピンリング");
        }
        bool canPunch = isInPunchTimerRange();
        if (!mMario->isSwimming()) {
            canPunch = true;
        }
        if (canPunch) {
            const Mario::MovementStates& moveStates = getMovementStates();
            if (moveStates._8 || moveStates._1A || moveStates._19) {
                mMario->tryWallPunch();
            }
        }
    }
    if (mMario->isAnimationRun("ハンマー投げリリース") && mMario->getMovementStates()._1 && !_38C && !mMario->_420 && mMario->Mario::isStickOn()) {
        mMario->stopAnimation(nullptr, (const char*)nullptr);
    }
}

bool MarioActor::doRush() {
    if (_934) {
        tryCoinPull();
        tryRushInRush();
        if (!_934) {
            return false;
        }
        updateGravityVec(false, false);
        if (!checkClapCatchStart() && _7DC) {
            bodyClap();
        }
        _7DC = 0;
        _7E2 = false;
        mMario->mPosition = mPosition;
        mMario->updateGroundInfo();
        mMario->updateCubeCode();
        mMario->decDamageAfterTimer();
        if (selectWaterInOutRush(_924)) {
            WaterInfo stack_44;
            if (!MR::getWaterAreaObj(&stack_44, mPosition)) {
                mMario->forceExitSwim();
            }
        } else if (!selectWaterInOutEffect(_924->mHost->mName)) {
            s32 initial = mMario->mSwim->mSwimState;
            mMario->mSwim->checkWaterCube(false);
            if (mMario->mSwim->mSwimState != initial) {
                if ((u32)mMario->mSwim->mSwimState <= MarioSwim::SWIM_STATE_ENTERING && (u32)initial - 2 <= 1) {
                    playEffectRT("水面ジャンプ水柱", mMario->mSwim->mSurfacePos, mMario->mSwim->mSurfaceNorm);
                    emitEffectWaterColumn(mMario->mSwim->mSurfacePos, mMario->mSwim->mSurfaceNorm);
                    // SWIM_STATE_UNDERWATER and SWIM_STATE_SURFACE
                } else if ((u32)initial <= 1 && (u32)(mMario->mSwim->mSwimState - MarioSwim::SWIM_STATE_UNDERWATER) <= 1) {
                    playEffectRT("水面ジャンプ水柱", -mMario->_328, mMario->mSwim->mSurfaceNorm);
                    emitEffectWaterColumn(mMario->mSwim->mSurfacePos, mMario->mSwim->mSurfaceNorm);
                }
                if (initial == 2) {
                    mWaterLife = 8;
                    MR::getGameSceneLayoutHolder()->changeLifeMeterModeGround();
                    mMario->forceExitSwim();
                } else if (initial == 0) {
                    MR::getGameSceneLayoutHolder()->changeLifeMeterModeWater();
                }
            }
        }
        if (mMario->isForceStopRush()) {
            bool received = _924->receiveMessage(0x95, getSensor("body"));
            if (_934 && received) {
                RushEndInfo stack_20(nullptr, 4, TVec3f(0.0f, 0.0f, 0.0f), false, 0);
                endRush(&stack_20);
            }
        }
        mMarioAnim->setHoming();
        updateTornado();
        if (_934 && selectRecoverFlyMeter(_924)) {
            for (u32 i = 0; i < 4; i++) {
                mMario->incAirWalkTimer();
            }
        }
        updateSwingTimer();
        return true;
    } else {
        _7E2 = false;
        return false;
    }
}

void MarioActor::updateSwingTimer() {
    if (_934) {
        stopSpinTicoEffect(false);
    } else {
        if (_948) {
            _948--;
        }
        if (_94C && --_94C == 6) {
            _94E = 5;
        }
        if (_94E && --_94E == 0) {
            mMario->startPadVib((u32)0);
            mMario->playSound("スピン回復終了", -1);
            Color8 stack_8(80, 128, 200, 0);
            _1AA = 0xf;
            _1AC = 1.5f;
            _1B0 = stack_8;
            _1B5 = true;
        }
        if (_946) {
            if (--_946 == 0x18) {
                selectAction("スピン回復エフェクト");
                stopEffectForce("スピンリング");
            }
            if (_946 == 0xd) {
                _94C = 0x13;
            }
        }
    }
}

void MarioActor::updateSwingAction() {
    if (isJumping() && mPlayerMode != 6 && !mMario->isStatusActive(MarioStatus_Foo)) {
        _946 = 0;
    }
    bool requestRush = isRequestRush();
    bool requestSpinJump2P = false;
    if (!requestRush) {
        if (requestRush = isRequestSpinJump2P()) {
            requestSpinJump2P = true;
        }
    }
    _1E1 = requestRush;
    if (_946) {
        if (!requestRush) {
            return;
        }
        if (!isPunching()) {
            if (_948) {
                _948 += 0x96;
            } else {
                _948 += 0x12c;
            }
        }
        if (!requestRush) {
            return;
        }
        tryReleaseBombTeresa();
        return;
    }
    bool canRush = true;
    if (!requestRush) {
        return;
    }
    if (mMario->isAnimationRun("壁はじき")) {
        canRush = false;
    }
    if (isJumping() && mMario->_428) {
        canRush = false;
    }
    if (mPlayerMode == 9) {
        canRush = false;
    }
    if (mMario->getCurrentStatus() == 1) {
        canRush = false;
    }
    if (mMario->isSwimming()) {
        canRush = false;
    }
    if (mMario->isStatusActive(MarioStatus_Foo)) {
        canRush = false;
    }
    if (mMario->isStatusActive(MarioStatus_13)) {
        canRush = false;
    }
    if (_468) {
        canRush = false;
    }
    if (mMario->isStatusActive(MarioStatus_Damage)) {
        canRush = false;
    }
    if (_3C0) {
        canRush = false;
    }
    if (_EA4) {
        canRush = false;
    }
    if (!canRush) {
        return;
    }
    u8 action = selectAction("スピンアタック");
    switch (action) {
    case 1:
        bool didSpinPunch = true;
        if (!mMario->mMovementStates._F && isJumping() && !mMario->isDamaging() && !mMario->mMovementStates._2B) {
            bool tmp = false;
            if (_F0C) {
                tmp = true;
            }
            mMario->trySpinJump(tmp);
            setPunchHitTimer(0x19);
            tryReleaseBombTeresa();
            if (requestSpinJump2P) {
                MR::start2PJumpAssistSound();
            }
        } else if (!getMovementStates()._F && !mMario->isAnimationRun("地上ひねり")) {
            const char* pLastAnimationName = mMarioAnim->mXanimePlayerUpper->getCurrentAnimationName();
            if (mPlayerMode == 4) {
                if (!mMario->isAnimationRun("ハチスピン")) {
                    didSpinPunch = trySpinPunch();
                }
            } else {
                didSpinPunch = trySpinPunch();
            }
            _974 = 0;
            if (pLastAnimationName != mMarioAnim->mXanimePlayerUpper->getCurrentAnimationName()) {
                mMario->playSound("パンチ風切り", -1);
            }
        }
        if (mPlayerMode == 4) {
            if (isJumping()) {
                if (!mMario->isAnimationRun("ハチスピン空中")) {
                    mMario->playSound("声スピン", -1);
                    mMario->playSound("スピンジャンプ", -1);
                }
                mMario->changeAnimation("ハチスピン空中", (const char*)nullptr);
            } else if (getMovementStates()._A || mAlphaEnable) {
                mMario->changeAnimation("サマーソルト", (const char*)nullptr);
            } else {
                mMario->changeAnimation("ハチスピン", (const char*)nullptr);
            }
        }
        if (didSpinPunch) {
            _946 = mConst->getTable()->mSpinIntervalTime + 0x22;
        }
        break;
    case 2:
        if (isEnableSpinPunch() && !mMario->isSwimming()) {
            shootFireBall();
        }
        break;
    case 3:
        if (isEnableSpinPunch()) {
            doFreezeAttack();
            setPunchHitTimer(0x1e);
        }
        break;
    case 4:
        if (mMario->_418 != 0) {
            break;
        }
        mMario->startTeresaDisappear();
        const MarioConstTable* pConstants = mConst->getTable();
        _946 = pConstants->mTeresaWallThroughTime + pConstants->mSpinIntervalTime;
        break;
    case 5:
        if (!isEnableSpinPunch()) {
            break;
        }
        if (isJumping()) {
            mMario->changeAnimation("ハチスピン空中", (const char*)nullptr);
        } else {
            if (getMovementStates()._A || mAlphaEnable) {
                mMario->changeAnimation("サマーソルト", (const char*)nullptr);  // Summersault
            } else {
                mMario->changeAnimation("ハチスピン", (const char*)nullptr);
            }
        }
        _946 = mConst->getTable()->mSpinIntervalTime + 0x22;
        break;
    }
}

bool MarioActor::checkClapCatchStart() {
    if (_6D0) {
        _6D0 = 0;
        tryCoinPullInRush();
        return true;
    } else {
        return false;
    }
}

void MarioActor::updateRealMtx() {
    updateBaseScaleMtx();
    getRealMtx(_C2C.toMtxPtr(), "HandL");
    getRealMtx(_C5C.toMtxPtr(), "HandR");
    PSMTXCopy(MR::getJointMtx(this, "HandL"), _C8C.toMtxPtr());
    PSMTXCopy(MR::getJointMtx(this, "HandR"), _CBC.toMtxPtr());
    PSMTXCopy(MR::getJointMtx(this, "Face0"), _CEC.toMtxPtr());
    getRealMtx(_D1C.toMtxPtr(), "FootR");
    getRealMtx(_D4C.toMtxPtr(), "Spine1");
    getRealMtx(_D7C.toMtxPtr(), "CapPosition");
    getRealMtx(_DAC.toMtxPtr(), "Hip");
    getRealMtx(_DDC.toMtxPtr(), "Spine2");
    TVec3f stack_50;
    MR::extractMtxTrans(_DDC.toMtxPtr(), &stack_50);
    if (MR::isNan(stack_50)) {
        MR::getJointMtx(this, "Spine2");
        getBaseMtx();
    }
    bool notStatus12 = true;
    if (mMario->isStatusActive(MarioStatus_FpView)) {
        notStatus12 = false;
    }
    if (!notStatus12) {
        return;
    }
    TVec3f stack_44, stack_38;
    MR::extractMtxTrans(_D7C.toMtxPtr(), &stack_44);
    MR::extractMtxTrans(_DAC.toMtxPtr(), &stack_38);
    _4B8 = stack_44 - stack_38;
    _4C4 = stack_38 - stack_44;
    if (MR::normalizeOrZero(&_4B8)) {
        _4B8 = mMario->mHeadVec;
    }
    if (MR::normalizeOrZero(&_4C4)) {
        _4C4 = -mMario->mHeadVec;
    }
    if (mMario->getMovementStates()._A && mMario->mMovementStates._1) {
        _4C4 = -mMario->mHeadVec;
    }
    _2AC = stack_44;
}

void MarioActor::setUpperRotateY(f32 angle) {
    mMarioAnim->setUpperRotateY(angle);
}

void MarioActor::decLife(u16 amt) {
    if (getMovementStates()._1F) {
        return;
    }
    if (_388 > 0 && amt > _388) {
        return;
    }
    if (amt) {
        _388 = amt;
        return;
    }
    if (mHealth) {
        mHealth--;
    }
    _388 = 0;
    if (gIsLuigi) {
        if (mMaxHealth == 3) {
            return;
        }
        if (mHealth > 3) {
            return;
        }
        mMaxHealth = 3;
        return;
    }
    if (mMaxHealth == 3) {
        return;
    }
    if (mHealth > 3) {
        return;
    }
    mMaxHealth = 3;
}

void MarioActor::decLifeLarge() {
    decLife(0);
}

void MarioActor::decLifeMiddle() {
    decLife(0);
}

void MarioActor::decLifeSmall() {
    decLife(0);
}

void MarioActor::resetWaterLife() {
    mWaterLife = 8;
}

void MarioActor::updateLife() {
    if (!_388) {
        return;
    }
    if (--_388 != 0) {
        return;
    }
    if (!mHealth) {
        return;
    }
    mHealth--;
}

static const char* sMiddleWaterLifeReduction = "水中ライフ減少";

void MarioActor::incLife(u32 amt) {
    if (isEnableNerveChange() && !_3E4) {
        const u32 health = getHealth();
        if (mHealth != mMaxHealth) {
            mMario->playSound("ライフ回復", -1);
        }
        mHealth += amt;
        if (mHealth >= mMaxHealth) {
            mHealth = mMaxHealth;
        }
        if (health == 1 && mMarioAnim->isAnimationStop()) {
            mMarioAnim->mXanimePlayer->changeTrackAnimation(3, "ノーマルウエイト");
            if (mMario->_970 && strcmp(mMario->_970, "DamageWait")) {
                mMario->startBas(nullptr, false, 0.0f, 0.0f);
                setBlink(nullptr);
            }
        }
    }
}

void MarioActor::changeMaxLife(s32 max) {
    mMaxHealth = max;
    while (mHealth != max) {
        if (mHealth > max) {
            decLife(0);
        } else if (mHealth < max) {
            incLife(1);
        } else {
            break;
        }
    }
}

bool MarioActor::doPressing() {
    if (!_390) {
        return false;
    }
    switch (_39C) {
    case 0:
        if (!mMario->checkVerticalPress(false)) {
            if (--_390 == 0x1e && !mHealth && isEnableNerveChange()) {
                setNerve(&NrvMarioActor::MarioActorNrvGameOver::sInstance);
            }
            if (!_390) {
                mMario->changeAnimation("つぶれ解除", (const char*)nullptr);
                _F44 = true;
            }
        }
        break;
    case 2:
    case 3:
        if (--_390 == 0x1e && !mHealth && isEnableNerveChange()) {
            setNerve(&NrvMarioActor::MarioActorNrvGameOver::sInstance);
        }
        if (!_390) {
            mMario->changeAnimation("つぶれ解除", (const char*)nullptr);
            _F44 = true;
        }
        break;
    case 4:
        if (_390 > 2) {
            _390--;
        }
        mMario->mMovementStates._8 = false;
        mMario->mMovementStates._19 = false;
        mMario->mMovementStates._1A = false;
        break;
    }
    if (_39C != 1) {
        careMovingObject();
    }
    mMarioAnim->setSpeed(0.0f);
    mMario->_350.zero();
    mMario->_35C.zero();
    resetSensorCount();
    return true;
}

void MarioActor::careMovingObject() {
    if (_1E0) {
        return;
    }
    if (getMovementStates()._1 && !MR::isSameMtx(mMario->_45C->getBaseMtx()->toMtxPtr(), mMario->_45C->getPrevBaseMtx()->toMtxPtr())) {
        mMario->mPosition = mPosition;
        mMario->checkEnforceMove();
        mPosition = mMario->mPosition;
    }
    mMario->mPosition = mPosition;
    mMario->mVelocity.zero();
    mMario->powerAreaMove();
    mMario->powerRailMove();
    mMario->mPosition += mMario->mVelocity;
    mMario->mVelocity.zero();
    mPosition = mMario->mPosition;
    mMario->checkGround();
    mMario->updateFloorCode();
}

bool MarioActor::doStun() {
    if (_38C) {
        mMario->mDrawStates._1A = true;
        resetSensorCount();
        if (getMovementStates()._1 && !MR::isSameMtx(mMario->_45C->getBaseMtx()->toMtxPtr(), mMario->_45C->getPrevBaseMtx()->toMtxPtr())) {
            mMario->mPosition = mPosition;
            mMario->checkEnforceMove();
            mPosition = mMario->mPosition;
        }
        if (!--_38C) {
            f32 stickX, stickY;
            getStickValue(&stickX, &stickY);
            if (MR::isNearZero(stickX) && MR::isNearZero(stickY) && !getMovementStates()._A) {
                mMario->stopWalk();
            }
        }
        mMario->inputStick();
        return true;
    } else {
        return false;
    }
}

void MarioActor::scaleMtx(MtxPtr rawMtx) {
    TVec3f i, j, k;
    const TRot3f* pMtx = (TRot3f*)rawMtx;
    f32 scalar = 0.35f * (1.0f - _3B0) + 1.0f;
    pMtx->getXDir(i);
    pMtx->getYDir(j);
    pMtx->getZDir(k);
    f32 elementX = MR::vecKillElement(i, _3B4, &i);
    f32 elementY = MR::vecKillElement(j, _3B4, &j);
    f32 elementZ = MR::vecKillElement(k, _3B4, &k);
    elementZ *= _3B0;
    elementX *= _3B0;
    elementY *= _3B0;
    i.scale(scalar);
    j.scale(scalar);
    k.scale(scalar);
    i += _3B4 * elementX;
    j += _3B4 * elementY;
    k += _3B4 * elementZ;
    rawMtx[0][0] = i.x;
    rawMtx[1][0] = i.y;
    rawMtx[2][0] = i.z;
    rawMtx[0][1] = j.x;
    rawMtx[1][1] = j.y;
    rawMtx[2][1] = j.z;
    rawMtx[0][2] = k.x;
    rawMtx[1][2] = k.y;
    rawMtx[2][2] = k.z;
}

void MarioActor::updateBaseScaleMtx() {
    if (mMario->mMovementStates._3C) {
        PSMTXScale(_BF8.toMtxPtr(), mScale.x, mScale.y, mScale.z);
    } else {
        PSMTXIdentity(_BF8.toMtxPtr());
    }
}

void MarioActor::getRealMtx(MtxPtr mtx, const char* pName) const {
    PSMTXConcat(MR::getJointMtx(this, pName), _BF8, mtx);
    TVec3f stack_8(mMario->_13C);
    MR::addTransMtx(mtx, stack_8);
}

void MarioActor::getRealPos(const char* pName, TVec3f* pVec) const {
    Mtx mtx;
    getRealMtx(mtx, pName);
    MR::extractMtxTrans(mtx, pVec);
}

MtxPtr MarioActor::getGlobalJointMtx(const char* pName) {
    u16 idx = MR::getJointIndex(this, pName);
    getRealMtx(_C28[idx], pName);
    return _C28[idx];
}

void MarioActor::calcAnimInMovement() {
    // FIXME: missing class at _4A4
    if (mHealth != 0 && (mMario->isDamaging() || mMario->getDamageAfterTimer())) {
        _1AA = 0;
        _1A8++;
    } else if (_1AA != 0) {
        _1AA--;
    }

    _1C0 = true;

    updateBlink();
    calcSpinEffect();

    switch (mCurrModel) {
    case 4:
        _9C8->movement();
        _A50->movement();
        _A54->movement();

        MR::setBrkFrame(_A50, MR::getBrkFrame(_9C8));
        MR::setBrkFrame(_A54, MR::getBrkFrame(_9C8));
    }

    if (_3DA != 0) {
        _3DA--;
        _1D4 = 10.0f;

        if (_3DA == 0) {
            MR::endDemo(this, "マリオスーパー化");
            MR::releaseAnimFrame(this);

            _1D4 = 0.0f;

            MR::explainLifeUpIfAtFirst();
        }
    }

    if (_3D8 != 0) {
        if (mPlayerMode == 6 && mMario->mVerticalSpeed < mConst->getTable()->mTeresaWaitHeight) {
            mPosition -= getGravityVec();
        }

        bool val = true;
        switch (mPlayerMode) {
        case 4:
            val = MR::isOnGameEventFlagBeeMarioAtFirst();
            break;
        case 6:
            val = MR::isOnGameEventFlagTeresaMarioAtFirst();
            break;
        case 5:
            val = MR::isOnGameEventFlagHopperMarioAtFirst();
            break;
        case 2:
            val = MR::isOnGameEventFlagFireMarioAtFirst();
            break;
        case 3:
            val = MR::isOnGameEventFlagIceMarioAtFirst();
            break;
        case 7:
            val = MR::isOnGameEventFlagFlyingMarioAtFirst();
            break;
        }

        if (val == false) {
            TVec3f frontVec;
            // MR::calcFrontVec(frontVec, _4A4->_24);
            mMario->setFrontVecKeepUp(frontVec);

            if (_336 == 0) {
                MR::zoomInTargetGameCamera();
                MR::startGlobalEventCamera("変身初出カメラ", CameraTargetArg(nullptr, nullptr, nullptr, this), 60);

                TVec3f vec(_2A0);

                f32 scale = 500.0f;
                if (mPlayerMode == 6) {
                    scale = 700.0f;
                }

                if ((mMario->mMovementStates_LOW_WORD >> 20 & 1) != 0 && (mMario->mMovementStates_LOW_WORD >> 30 & 1) == 0) {
                    vec += mMario->mJumpVec;
                }

                MR::setProgrammableCameraParam("変身初出カメラ", vec, vec + frontVec * scale, -_240, true);
            }

            _336 = 1;
        }

        if (--_3D8 == 0) {
            if (mTransforming) {
                MR::endDemo(this, "マリオ変身");
                mTransforming = false;
            }

            MR::releaseAnimFrame(this);
            _3DE = 1;

            switch (mPlayerMode) {
            case 1:
                MR::explainBeeMarioIfAtFirst();
                break;
            case 2:
                MR::explainTeresaMarioIfAtFirst();
                break;
            case 3:
                MR::explainHopperMarioIfAtFirst();
                break;
            case 4:
                MR::explainFireMarioIfAtFirst();
                break;
            case 5:
                MR::explainIceMarioIfAtFirst();
                break;
            case 6:
                MR::explainFlyingMarioIfAtFirst();
                break;
            case 7:
                MR::explainInvincibleMarioIfAtFirst();
                break;
            }

            if (_336 != 0) {
                _338 = 10;
            }

            _336 = 0;
        }

        _3DF = 1;
    } else {
        mMario->updateMorphResetTimer();
    }

    if (_338 != 0 && --_338 == 0) {
        MR::zoomOutTargetGameCamera();
        MR::endGlobalEventCamera("変身初出カメラ", 60, true);
    }
}

void MarioActor::forceSetBaseMtx(MtxPtr mtx) {
    _EA5 = true;
    _1C0 = true;
    PSMTXCopy(mtx, _EA8);
    MR::extractMtxTrans(mtx, &_2F4);
    if (_482) {
        MR::extractMtxTrans(mtx, &mPosition);
    }
    ((TRot3f*)mtx)->getZDir(_2DC);
    ((TRot3f*)mtx)->getYDir(_2D0);
    ((TRot3f*)mtx)->getXDir(_2E8);
    MR::updateHitSensorsAll(this);
    mMario->invalidateRelativePosition();
    mMario->_8F8.zero();
    mMario->mMovementStates._23 = 0;
}

void MarioActor::calcAnim() {
    // FIXME: switch stuff
    // https://decomp.me/scratch/Xf6TH
    if (_482 != 0) {
        mBlendMtxTimer = 0;
        return;
    }

    if (_3DF != 0) {
        u16 val = (_3D8 & 7) < (_3D8 >> 3) ? _3D6 : mPlayerMode;
        if (_3D8 != 0 && mPlayerMode == 0) {
            if ((s32)val != (s32)mPlayerMode && _3D6 == 6) {
                if (26 < _3D8) {
                    _9A4->appear();
                }

                _483 = 1;
                updateHand();
                updateFace();
                return;
            }

            if (_3D6 == 6) {
                _9A4->kill();
                _483 = 0;
            }
        }

        u32 frame;
        switch (val) {
        case 0:
            changeDisplayMode(2);
            break;
        case 1:
            changeDisplayMode(4);
            _9C8->calcAnim();

            if (_3DE != 0) {
                _A6E = 2;
            }
            break;
        case 2:
            changeDisplayMode(0);
            frame = 0;
            if (val == 7) {
                frame = 1;
            } else if ((val < 7) && val == 2) {
                frame = 2;
            }
            MR::startBtp(this, "ColorChange");
            MR::setBtpFrameAndStop(this, frame);
            break;
        case 3:
            changeDisplayMode(3);
            break;
        case 4:
            changeDisplayMode(10);
            break;
        case 5:
            _3DF = 0;
            changeDisplayMode(5);
            break;
        case 6:
            changeDisplayMode(0);
            frame = 0;
            if (val == 7) {
                frame = 1;
            } else if ((val < 7) && val == 2) {
                frame = 2;
            }
            MR::startBtp(this, "ColorChange");
            MR::setBtpFrameAndStop(this, frame);
            break;
        }

        if (_3DF != 0) {
            changeHandMaterial();
        }
    }

    if (mMario->isStatusActive(MarioStatus_Paralyze)) {
        if (mCurrModel != 1) {
            _A0B = mCurrModel;
        }

        if ((_37C & 3) != 0) {
            changeDisplayMode(1);
        } else {
            changeDisplayMode(_A0B);
        }
    } else if (mCurrModel == 1 && mHealth != 0) {
        changeDisplayMode(_A0B);
    }

    _3DF = 0;
    _3DE = 0;

    calcAndSetBaseMtx();
    mMarioAnim->switchMirrorMode();
    mMarioAnim->calc();

    if (_3D8 == 0) {
        switch (mPlayerMode) {
        case 4:
            _9E4->mPosition.set(mPosition);

            MR::updateModelDiffDL(_9E4);
            break;
        case 3:
            _9C0->mPosition.set(mPosition);

            MR::updateModelDiffDL(_9C0);
            MR::updateModelDiffDL(_A40);
            MR::updateModelDiffDL(_A44);
            break;
        case 2:
            _9C8->mPosition.set(mPosition);

            MR::updateModelDiffDL(_9C8);
            MR::updateModelDiffDL(_A50);
            MR::updateModelDiffDL(_A54);
            break;
        case 5:
            _A00->mPosition.set(mPosition);

            MR::updateModelDiffDL(_A00);
            break;
        case 1:
            if (mCurrModel != 1) {
                MR::updateModelDiffDL(this);
            }
            break;
        }
    } else if (mCurrModel == 0) {
        MR::updateModelDiffDL(this);
    }

    if (MR::isHiddenModel(this) && _482 == 0) {
        MR::showModel(this);
    }

    updateRealMtx();

    if ((mMario->mMovementStates_LOW_WORD >> 16 & 1) == 0 || mMario->_544 <= 1) {
        if (mAlphaEnable != 0) {
            _214->setMode(0);
        } else {
            _214->setMode(2);
        }

        TVec3f shadowPos(mMario->mShadowPos);
        shadowPos -= mPosition;
        _214->_20 = 40.0f + shadowPos.dot(_240);
        _214->_48 = mMario->mShadowPos;

        TMtx34f leftMtx;
        TVec3f leftVec;
        getRealMtx(leftMtx, "HandL");
        MR::extractMtxTrans(leftMtx, &leftVec);

        TMtx34f rightMtx;
        TVec3f rightVec;
        getRealMtx(rightMtx, "HandR");
        MR::extractMtxTrans(rightMtx, &rightVec);

        TVec3f stack114(rightVec);
        stack114 -= leftVec;

        f32 length = stack114.length();
        MR::normalizeOrZero(&stack114);

        TVec3f cross;
        cross.cross(stack114, _240);
        MR::normalizeOrZero(&cross);

        stack114.cross(_240, cross);

        if (MR::normalizeOrZero(&stack114)) {
            stack114 = mMario->mSideVec;
        }

        if (mMario->isSwimming()) {
            CollisionShadow* pShadow = _214;
            pShadow->_3C = stack114;
            pShadow->_18 = 10.0f + length;
            pShadow->_1C = 140.0f;
        } else {
            CollisionShadow* pShadow = _214;
            pShadow->_3C = stack114;
            pShadow->_18 = 5.0f + length;
            pShadow->_1C = 70.0f;
        }

        TVec3f vec;
        if (mMario->isStatusActive(MarioStatus_Hang)) {
            TMtx34f mtx;
            getRealMtx(mtx, "Center");
            MR::extractMtxTrans(mtx, &vec);
        } else {
            TMtx34f mtx;
            getRealMtx(mtx, "All_Root");
            MR::extractMtxTrans(mtx, &vec);
        }

        _214->create(vec, _240, mMario->mFrontVec);
    }

    if (_1C1 == 0) {
        mMario->updateLookOfs();
    }

    updateHand();
    updateFace();
    calcScreenBoxRange();

    if (_946 || _F20 || _F21 || mMario->mMovementStates_HIGH_WORD >> 20 & 1) {
        _94A = 0;
    } else {
        _94A++;
    }

    if (mCurrModel != 0) {
        PSMTXCopy(MR::getJointMtx(getJ3DModel(), "HandL0"), MR::getJointMtx(mModels[0], "HandL0"));
        PSMTXCopy(MR::getJointMtx(getJ3DModel(), "HandR0"), MR::getJointMtx(mModels[0], "HandR0"));
        PSMTXCopy(MR::getJointMtx(getJ3DModel(), "FootL"), MR::getJointMtx(mModels[0], "FootL"));
        PSMTXCopy(MR::getJointMtx(getJ3DModel(), "FootR"), MR::getJointMtx(mModels[0], "FootR"));
        PSMTXCopy(MR::getJointMtx(getJ3DModel(), "Center"), MR::getJointMtx(mModels[0], "Center"));
        PSMTXCopy(MR::getJointMtx(getJ3DModel(), "All_Root"), MR::getJointMtx(mModels[0], "All_Root"));
        PSMTXCopy(MR::getJointMtx(getJ3DModel(), "Spine1"), MR::getJointMtx(mModels[0], "Spine1"));
        PSMTXCopy(MR::getJointMtx(getJ3DModel(), "Hip"), MR::getJointMtx(mModels[0], "Hip"));
        PSMTXCopy(MR::getJointMtx(getJ3DModel(), "Face0"), MR::getJointMtx(mModels[0], "Face0"));

        PSMTXCopy(getJ3DModel()->mBaseTransformMtx, mModels[0]->mBaseTransformMtx);
    }

    updateRasterScroll();
}

void MarioActor::calcAndSetBaseMtx() {
    // FIXME: biiiig mess, barely got started
    // https://decomp.me/scratch/QGstP
    if (_1C0 == false) {
        _1C1 = true;

        return;
    }

    _1C1 = false;
    _1C0 = false;

    TMtx34f mtx;
    mtx.set(getJ3DModel()->getBaseTRMtx());

    if (_934 && receiveMessage(ACTMES_UPDATE_BASEMTX, _924, getSensor("body"))) {
        TVec3f headVec;
        reinterpret_cast< TRot3f* >(getBaseMtx())->getYDir(headVec);
        MR::normalizeOrZero(&headVec);
        mMario->setHeadVec(headVec);

        TVec3f frontVec;
        reinterpret_cast< TRot3f* >(getBaseMtx())->getZDir(frontVec);
        MR::normalizeOrZero(&frontVec);
        mMario->setFrontVecKeepUp(frontVec);
        mMario->_334 = frontVec;
    }

    bool b1;
    if (_EA4 || _EA5) {
        if (_EA5 == false && _EA6) {
            return;
        }

        PSMTXCopy(_EA8, getBaseMtx());

        TVec3f headVec;
        reinterpret_cast< TRot3f* >(getBaseMtx())->getYDir(headVec);
        mMario->setHeadVec(headVec);

        TVec3f frontVec;
        reinterpret_cast< TRot3f* >(getBaseMtx())->getZDir(frontVec);
        mMario->setFrontVecKeepUp(frontVec);

        mMario->_278 = 0.0f;
        mMario->_71C = 0;
        mMario->stopJump();
        mMario->_334 = mMario->mFrontVec;

        if (mMario->isSwimming()) {
            mMario->mSwim->resetAndFixPose();
        }

        b1 = true;
        if (_EA4) {
            _EA6 = true;
        }
    }

    if (b1) {
        TVec3f vec(mPosition);
        MR::extractMtxTrans(getBaseMtx(), &mPosition);
        mMario->invalidateRelativePosition();

        _938 = -mPosition;

        TMtx34f mtx;
        PSMTXCopy(getBaseMtx(), mtx);

        if (mMario->mMovementStates_HIGH_WORD >> 8 & 1) {
            mMario->_688 = mPosition;
        }

        updateGravityVec(true, true);
        mMario->updateGroundInfo();
    }

    if (_934 && !b1) {
        TVec3f vec;
        MR::extractMtxTrans(getBaseMtx(), &vec);
        _938 = mPosition - vec;
    }

    TMtx34f mtx2;
    mtx2.identity();

    mMario->createAngleMtx(mtx2, b1);

    if (_EA6) {
        TMtx34f capMtx;
        getRealMtx(capMtx, "CapPosition");
        MR::extractMtxTrans(capMtx, &_2AC);

        if ((_2AC - mPosition).length() > 500.0f) {
            PSMTXCopy(getBaseMtx(), mtx2);
            MR::setMtxTrans(mtx2, 0.0f, 0.0f, 0.0f);
        }
    }

    PSMTXConcat(mtx2, MR::tmpMtxRotYRad(mMario->mYAngleOffset), mtx2);

    if (mPlayerMode == 6) {
        // updateBaseMtxTeresa(mtx2);
    }

    TPos3f mtxD8;
    if (!b1 && mPlayerMode == 4 &&
            !(mMario->isStatusActive(MarioStatus_Stick) || mMario->isStatusActive(MarioStatus_SideStep) ||
              mMario->isStatusActive(MarioStatus_Flip)) &&
            !(mMario->mMovementStates_HIGH_WORD >> 28 & 1 || mMario->mMovementStates_LOW_WORD >> 21 & 1) ||
        ((mMario->mMovementStates_LOW_WORD >> 20 & 1) && (mMario->mMovementStates_LOW_WORD >> 31 & 1))) {
        _9F4 = mMario->mAirGravityVec;

        TVec3f vec(-getGravityVec());
        vec.scale(mConst->getTable()->mBeePoseHeadToFootLength);
        mtxD8 = MR::tmpMtxTrans(-vec);

        TVec3f vec2(_9F4);
        vec2.scale(mConst->getTable()->mBeePoseHeadToFootLength);

        TVec3f vec3(mPosition);
        vec3 -= vec2;

        TVec3f vec4(_33C);
        vec4 -= vec3;

        TVec3f vec5;

        f32 dot = 0.0f;
        if (!MR::normalizeOrZero(&vec4)) {
            vec5.cross(vec4, mMario->mFrontVec);

            if (!MR::normalizeOrZero(&vec5)) {
                f32 val = MR::acosEx(_9F4.dot(vec4));
                f32 max = mConst->getTable()->mBeePoseDelayAngleAir;
                if (mMario->mMovementStates_LOW_WORD >> 30 & 1) {
                    max = mConst->getTable()->mBeePoseDelayAngleGround;
                }
                dot = MR::clamp(val, 0.0f, max);
            }
        }
        MtxPtr mtx3;
        PSMTXRotAxisRad(mtx3, &vec5, dot);
        PSMTXConcat(mtxD8, mtx2, mtx2);
        PSMTXConcat(mtx3, mtx2, mtx2);
        PSMTXConcat(MR::tmpMtxTrans(vec), mtx2, mtx2);

        TVec3f vec240(_9F4);
        vec240.scale(mConst->getTable()->mBeePoseDelayAccel);
        MR::vecKillElement(vec240, _360, &vec240);

        _354 += vec240;
        _33C += _354;

        TVec3f vec300(_9F4);
        vec300.scale(mConst->getTable()->mBeePoseHeadToFootLength);

        TVec3f vec2F4(mPosition);
        vec2F4 -= vec300;

        TVec3f vec2E8(_33C);
        vec2E8 -= vec2F4;
        _360 = vec2E8;

        TVec3f vec24C(_360);
        TVec3f vec258(_360);
        MR::normalizeOrZero(&vec258);

        f32 dot2 = MR::acosEx(_9F4.dot(vec258));

        f32 val = mConst->getTable()->mBeePoseLimitAngleAir;
        if (mMario->mMovementStates_LOW_WORD >> 30 & 1) {
            val = mConst->getTable()->mBeePoseLimitAngleGround;
        }

        if (val < dot2) {
            TVec3f vec264;
            vec264.cross(_9F4, vec258);

            if (!MR::normalizeOrZero(&vec264)) {
                _360 = _9F4;
            } else {
                MtxPtr mtx;
                PSMTXRotAxisRad(mtx, vec264, val);
                PSMTXMultVec(mtx, _9F4, _360);
            }
        }

        _360.setLength(mConst->getTable()->mBeePoseHeadToFootLength);

        if (mConst->getTable()->mBeePoseHeadToFootLength < vec24C.length()) {
            TVec3f vec318(vec24C);
            vec318 -= _360;
            _354 += -vec318;
        }

        TVec3f vec33C(_9F4);
        vec33C.scale(mConst->getTable()->mBeePoseHeadToFootLength);

        TVec3f vec330(mPosition);
        vec330 -= vec33C;
        _33C = _360 + vec330;

        MR::normalizeOrZero(&_360);

        if (mConst->getTable()->mBeePoseDelaySpeedLimit < _354.length()) {
            _354.setLength(mConst->getTable()->mBeePoseDelaySpeedLimit);
        }

        f32 friction = mConst->getTable()->mBeePoseFrictionStop;
        if (mMario->_71C == 0 && !isJumping()) {
            friction = mConst->getTable()->mBeePoseFrictionMove;
        }

        _354.scale(friction);

        if (MR::isSameDirection(_360, mMario->mFrontVec, 0.01f)) {
            if (MR::isSameDirection(mMario->_1FC, mMario->mFrontVec, 0.01f)) {
                MR::makeMtxUpFront(&mtxD8, mMario->mHeadVec, mMario->mFrontVec);
            } else {
                MR::makeMtxUpFront(&mtxD8, mMario->_1FC, mMario->mFrontVec);
            }
        } else {
            MR::makeMtxUpFront(&mtxD8, -_360, mMario->mFrontVec);
        }

        if (mMario->_71C < 3 && !isJumping()) {
            _348.scale(mConst->getTable()->mBeePoseTransBlendingRatioStop);
            MR::setMtxTrans(mtxD8, _348);
        } else {
            TVec3f vec378(_33C);
            vec378 -= mPosition;

            TVec3f vec36C(vec378);
            vec36C.scale(1.0f - mConst->getTable()->mBeePoseTransBlendingRatioMove);

            TVec3f vec360(_348);
            vec360.scale(mConst->getTable()->mBeePoseTransBlendingRatioMove);

            _348 += vec36C;

            MR::setMtxTrans(mtxD8, _348);
        }
    } else {
        _354.zero();
        _348.zero();
        _33C = mPosition;
        _9F4 = getGravityVec();
    }

    MR::addTransMtx(mtxD8, mPosition);

    if (!b1) {
        MtxPtr mtx198;
        TVec3f vec270;
        mMario->createCorrectionMtx(mtx198, &vec270);

        mMarioAnim->mXanimePlayer->mCore->getJointTransform(1)->_2C = vec270;

        PSMTXCopy(mtx198, _E3C);
        mMarioAnim->mXanimePlayer->mCore->getJointTransform(0)->_64 = _E3C;
    } else {
        MtxPtr mtx168;
        MtxPtr mtxA8;
        PSMTXInverse(mtxD8, mtx168);
        PSMTXConcat(mtx168, mtxA8, _E3C);
        mMarioAnim->mXanimePlayer->mCore->getJointTransform(0)->_64 = _E3C;
    }

    TMtx34f pMtx;
    if (_390 == 0) {
        if (_394 == 0) {
            if (mMario->getMovementStates()._3C) {
                _3B0 = 1.0f;
            }

            _394--;

            switch (_39C) {
            case 0:
            case 2:
                _3B0 = 1.0f + (0.15f * _394 / 30.0f) * MR::sin(2.0f * _394 * PI / 15.0f);
                break;
            case 1:
                _3B0 = 1.0f + (0.15f * _394 / 30.0f) * MR::sin(2.0f * _394 * PI / 15.0f);
                break;
            }
        }
    } else {
        _394 = 30;

        switch (_39C) {
        case 0:
        case 2:
            if (_390 < 16) {
                _3B0 = 0.2f + 0.8f * (15 - _390) / 15.0f;
            } else {
                f32 val = MR::clamp(mMario->calcDistToCeil(false) / 150.0f, 0.2f, 1.0f);

                if (_3B0 <= val) {
                    val = MR::clamp(mMario->calcDistToCeilOnPress() / 150.0f, 0.2f, 1.0f);
                    if (val < _3B0) {
                        _3B0 = val;
                    }

                    MR::setMtxTrans(pMtx, mPosition.x, mPosition.y, mPosition.z);

                    if (mMario->_960 != 27) {
                        _1E0 = true;
                    }
                } else {
                    _3B0 = val;
                }
            }

            if (_398 != 0) {
                _3B0 += 0.02f * _398;
            }
            break;

        case 1:
        case 3:
            if (_390 < 16) {
                _3B0 = 0.2f + 0.8f * (15 - _390) / 15.0f;
            } else {
                mPosition = mMario->mPosition;

                f32 val = MR::clamp(mMario->calcDistWidth() / 80.0f, 0.2f, 1.0f);

                if (_3B0 <= val) {
                    f32 oldVal = _3B0 - 0.1f;
                    _3B0 = oldVal;
                    _3B0 = MR::clamp(oldVal, 0.2f, 1.0f);
                } else {
                    _3B0 = val;
                }
            }

            if (_398 != 0) {
                _3B0 += 0.2f * _398;
            }
            break;

        case 4:
            f32 val = MR::negateIfLessZero(0.5f * _390 / 120.0f * PI);
            _3B0 = val;
        }
    }

    if (_398 != 0) {
        _398--;
    }

    u16 val = mBlendMtxTimer;
    if (val != 0) {
        _EA0 = 1.0f - (1.0f - _EA0) * (val - 1) / val;

        MtxPtr pMtx2;
        MR::blendMtx(pMtx2, pMtx, _EA0, pMtx);
        mBlendMtxTimer--;
    }

    _3EC = pMtx;

    if (_3B0 != 1.0f) {
        scaleMtx(pMtx);
    }

    PSMTXCopy(pMtx, getJ3DModel()->mBaseTransformMtx);
    getJ3DModel()->mBaseScale = mScale;

    _EA5 = true;
}

void MarioActor::setBlendMtxTimer(u16 a1) {
    if (a1 == 0) {
        _EA0 = 1.0f;
    } else {
        _EA0 = 0.0f;
    }
    mBlendMtxTimer = a1;
}

void MarioActor::getGroundPos(TVec3f* dst) const {
    *dst = mMario->mGroundPos;
}

TVec3f* MarioActor::getShadowPos() const {
    return &mMario->mShadowPos;
}

bool MarioActor::isTurning() const {
    return getMovementStates().turning;
}

bool MarioActor::isJumping() const {
    if (_934) {
        return _938.dot(getGravityVec()) < -10.0f;
    }

    if (mPlayerMode == 6 && mMario->mVerticalSpeed < mConst->getTable()->mTeresaDropDownHeight) {
        return false;
    }

    if (mMario->isWalling()) {
        return true;
    }

    return mMario->mMovementStates.jumping && !mMario->mMovementStates._1;
}

bool MarioActor::isJumpRising() const {
    return mMario->mMovementStates.jumping && mMario->isRising();
}

bool MarioActor::isPunching() const {
    if (mMario->isStatusActive(MarioStatus_Magic)) {
        return true;
    }

    if (mMario->mSwim->check7Aand7C()) {
        return true;
    }

    if (_3E5) {
        return true;
    }

    if (!mMario->isAnimationTerminate(nullptr) &&
        (mMario->isAnimationRun("空中ひねり") || mMario->isAnimationRun("アイスひねり") || mMario->isAnimationRun("アイスひねり空中") ||
         mMario->isAnimationRun("アイスひねり静止") || mMario->isAnimationRun("アイスひねり移動"))) {
        return true;
    }

    return false;
}

bool MarioActor::isItemSwinging() const {
    if (mMario->isAnimationRun("テニスショット左") || mMario->isAnimationRun("テニスショット右") || mMario->isAnimationRun("テニスショット中")) {
        return true;
    } else {
        return false;
    }
}

bool MarioActor::isNeedDamageFog() const {
    if (!isEnableNerveChange()) {
        return false;
    }
    if (MR::isDemoActive()) {
        return false;
    }
    if (_EA4) {
        return false;
    }
    return mMario->getDamageAfterTimer() != 0;
}

bool MarioActor::isDamaging() const {
    return mMario->isDamaging() != false;
}

bool MarioActor::isStaggering() const {
    return mMario->isStatusActive(MarioStatus_Flip) != false;
}

bool MarioActor::isSleeping() const {
    // No way that this is a real inline but only way I could make it match
    bool out = true;
    if (!IsMarioAnimationRun("特殊ウエイト1B") && !IsMarioAnimationRun("特殊ウエイト1A")) {
        out = false;
    }

    return out;
}

bool MarioActor::isRefuseTalk() const {
    // FIXME: isPlayerModeTeresa() is giving issues
    if (!isEnableNerveChange()) {
        return true;
    }

    if (mMario->isStatusActive(MarioStatus_Wait)) {
        return true;
    }

    if (mMario->isStatusActive(MarioStatus_Hang)) {
        return true;
    }

    if (mMario->isStatusActive(MarioStatus_13)) {
        return true;
    }

    if (mMario->isDamaging()) {
        return true;
    }

    if (mMario->_420 != 0) {
        return true;
    }

    if (mMario->isPlayerModeTeresa()) {
        if (_B94 != 0) {
            return true;
        }

        if (mMario->getMovementStates().jumping) {
            return true;
        }

        if (!mMario->getMovementStates()._1) {
            return true;
        }

        if (mMario->_3CE < 5) {
            return true;
        }

        if (mMario->getMovementStates()._23) {
            return true;
        }

        if (mMario->getDrawStates()._A) {
            return true;
        }

        if (mMario->_3D2 != 0) {
            return true;
        }
    }

    if (_424 != nullptr) {
        return true;
    }

    return _480;
}

bool MarioActor::isDebugMode() const {
    return getMovementStates().debugMode;
}

void MarioActor::jumpHop() {
    // https://decomp.me/scratch/d3cJS
    if (!isJumping()) {
        return;
    }

    if (!mMario->isRising()) {
        mMario->cutGravityElementFromJumpVec(true);

        mMario->mJumpVec += _240 * getConst().getTable()->mClapJumpBonusFalling;
    } else {
        f32 val = MR::min(getConst().getTable()->mHopLimit, -mMario->cutGravityElementFromJumpVec(true));
        // smth missing here
        mMario->mJumpVec += _240 * val;
    }

    if (mMario->_430 == 5) {
        mMario->_430 = 0;
        mMario->mMovementStates._1B = false;

        changeAnimation(nullptr, "落下");
    }

    if (mMario->_430 == 11) {
        mMario->_430 = 0;
    }

    mMario->_4B0 = mMario->mPosition;
}

void MarioActor::calcCenterPos() {
    // FIXME: optimized floats (needs to be separate)
    // https://decomp.me/scratch/oy8tV
    if (_934 || _8C) {
        MR::copyJointPos(this, "Spine1", &_2A0);

        return;
    }

    TVec3f vec24;
    if (mMario->isStatusActive(MarioStatus_Wall)) {
        vec24 = mMario->_75C;
    } else {
        if (mMario->isStatusActive(MarioStatus_Hang)) {
            MR::copyJointPos(this, "Spine1", &_2A0);

            return;
        }

        if (mMario->isSwimming()) {
            vec24 = _4B8;
        } else {
            vec24 = -getGravityVec();
        }
    }

    f32 val;
    if (mMario->getMovementStates()._A) {
        val = 0.8f * mBinder->mRadius + (1.0f - 0.8f) * 40.0f;
        mBinder->mRadius = val;
    } else {
        val = 0.9f * mBinder->mRadius + (1.0f - 0.9f) * 60.0f;
        mBinder->mRadius = val;
    }

    if (mPlayerMode == 6) {
        f32 val2 = 0.8f * mBinder->mRadius + (1.0f - 0.8f) * 100.0f;
        val = 20.0f + val2;
        mBinder->mRadius = val2;
    }

    _2A0 = mPosition + vec24 * val;
}

void MarioActor::calcHeadPos() {
    TMtx34f mtx;
    const char* pName = "CapPosition";
    getRealMtx(mtx, pName);
    MR::extractMtxTrans(mtx, &_2AC);
}

void MarioActor::setPress(u8 myChar, s32 myInt) {
    if (_390 != 0) {
        return;
    }

    if (myChar == 0 && mMario->calcDistToCeil(false) >= 160.0f) {
        return;
    }

    if (mMario->_10._1D) {
        return;
    }

    if (_39C == 0 || _39C == 2) {
        // Missing instruction
        *mMario->_480 = *mMario->mGroundPolygon;
        *mMario->_484 = *mMario->_4C8;
    }

    if (mMario->isStatusActive(MarioStatus_Blown)) {
        mMario->closeStatus(nullptr);
    }

    mMario->startPadVib("マリオ[つぶれ]");

    MR::forceDeleteEffectAll(this);

    mVelocity.zero();

    if (myInt == 0) {
        myInt = 240;
    }

    _390 = myInt;

    mMario->mMovementStates._B = false;
    mMario->mMovementStates._F = false;

    mMario->stopJump();

    mMario->mMovementStates._1 = true;

    mMario->_13C.zero();
    mMario->_148.zero();
    mMario->_154.zero();

    mMario->mVerticalSpeed = 0.0f;

    mMario->stopWalk();

    mMarioAnim->setSpeed(0.0f);

    _B90 = true;
    _39C = myChar;
    _F44 = false;

    mMario->playSound("プレスダメージ", -1);
    mMario->playSound("声大ダメージ", -1);

    _FB4 = &NrvMarioActor::MarioActorNrvGameOver::sInstance;
    _FB8 = 60;

    if (_39C != 4) {
        _390 = 1000;
    }

    for (u32 idx = 0; idx < 6; idx++) {
        decLife(0);
    }

    MR::startStarPointerModeDemoMarioDeath(this);
}

XanimeResourceTable* MarioActor::getResourceTable() const {
    return mMarioAnim->mResourceTable;
}

bool MarioActor::isEnableMoveMario() const {
    if (isNerve(&NrvMarioActor::MarioActorNrvWait::sInstance)) {
        return true;
    }

    if (isNerve(&NrvMarioActor::MarioActorNrvNoRush::sInstance)) {
        return true;
    }

    if (isNerve(&NrvMarioActor::MarioActorNrvGameOverNonStop::sInstance)) {
        return true;
    }

    return false;
}

bool MarioActor::isEnableNerveChange() const {
    if (isNerve(&NrvMarioActor::MarioActorNrvWait::sInstance)) {
        return true;
    }

    if (isNerve(&NrvMarioActor::MarioActorNrvNoRush::sInstance)) {
        return true;
    }

    return false;
}

void MarioActor::forceGameOver() {
    if (!isEnableNerveChange()) {
        return;
    }

    setNerve(&NrvMarioActor::MarioActorNrvGameOver::sInstance);
}

void MarioActor::forceGameOverAbyss() {
    if (!isEnableNerveChange()) {
        return;
    }

    setNerve(&NrvMarioActor::MarioActorNrvGameOverAbyss::sInstance);
}

void MarioActor::forceGameOverBlackHole() {
    if (!isEnableNerveChange()) {
        return;
    }

    setNerve(&NrvMarioActor::MarioActorNrvGameOverBlackHole::sInstance);
}

void MarioActor::forceGameOverNonStop() {
    if (!isEnableNerveChange()) {
        return;
    }

    setNerve(&NrvMarioActor::MarioActorNrvGameOverNonStop::sInstance);
}

void MarioActor::forceGameOverSink() {
    if (!isEnableNerveChange()) {
        return;
    }

    setNerve(&NrvMarioActor::MarioActorNrvGameOverSink::sInstance);
}

void MarioActor::updateCameraInfo() {
    _F74 = false;

    if (MR::diffAngleAbs(mCamDirZ, MR::getCamZdir()) >= PI / 6.0f) {
        _F74 = true;
    }

    if ((mCamPos - MR::getCamPos()).length() > 500.0f) {
        _F74 = true;
    }

    if (_F74) {
        mMario->changeAnimationInterpoleFrame(0);
        stopSpinTicoEffect(true);
        mBlendMtxTimer = 0;
    }

    mCamPos = MR::getCamPos();
    mCamDirX = MR::getCamXdir();
    mCamDirY = MR::getCamYdir();
    _FA8 = mCamDirZ;
    mCamDirZ = MR::getCamZdir();
}

bool MarioActor::binderFilter(const Triangle* pTriangle) {
    if (_F48 != nullptr && pTriangle->mSensor == _F48) {
        return true;
    }

    return false;
}

namespace MR {
    s32 getFrameBufferWidth() {
        return JUTGetVideoManager()->getRenderMode()->fbWidth;
    }
};  // namespace MR

void MarioActor::setPunchHitTimer(u8 punchHitTime) {
    if (_944 == 0) {
        _945 = 0;
        _974 = 0;
    }

    _944 = punchHitTime;
}
