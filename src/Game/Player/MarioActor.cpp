#include "Game/Player/MarioActor.hpp"
#include "Game/Animation/XanimePlayer.hpp"
#include "Game/Enemy/KariKariDirector.hpp"
#include "Game/Gravity.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Player/J3DModelX.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Map/WaterInfo.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioEffect.hpp"
#include "Game/Player/MarioHolder.hpp"
#include "Game/Player/MarioMessenger.hpp"
#include "Game/Player/ModelHolder.hpp"
#include "Game/Player/MarioNullBck.hpp"
#include "Game/Player/MarioParts.hpp"
#include "Game/Player/MarioShadow.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Player/RushEndInfo.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/FootPrint.hpp"
#include "JSystem/JAudio2/JAIAudible.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "JSystem/JUtility/JUTVideo.hpp"

bool gIsLuigi;

struct CameraFrontTarget {
    u8 _00[0x24];
    const LiveActor* mActor;
};

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
    _46C = nullptr;
    _470 = nullptr;
    _474 = nullptr;
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

    PSMTXIdentity(_EA8.toMtxPtr());
    PSMTXIdentity(_3EC.toMtxPtr());

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
    _424 = nullptr;
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

    PSMTXIdentity(_C2C.toMtxPtr());
    PSMTXIdentity(_C5C.toMtxPtr());
    PSMTXIdentity(_D1C.toMtxPtr());
    PSMTXIdentity(_D4C.toMtxPtr());
    PSMTXIdentity(_D7C.toMtxPtr());

    _1B4 = 0;
    _1C3 = false;
    _1E0 = false;
}

MarioActor::~MarioActor() {
    delete mMario;
}

static f32 BASE_ROTATION = 0.0f;

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

    // Matrix?
    _C28 = new DUMMY[MR::getJointNum(this)];

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
        mMario->changeAnimation("基本", static_cast< const char* >(nullptr));
        break;
    case 2:
        mMario->changeAnimationNonStop("ウォークイン");
        break;
    default:
        mMario->changeAnimation("ステージインA", static_cast< const char* >(nullptr));
        break;
    }
    updateTransForCamera();
    _F44 = 1;
    _984 = 0.0f;
    _978.zero();
    _27C.zero();
    _288.zero();
    _498 = new FixedPosition(this, "HandR", TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
    _49C = new FixedPosition(this, "HandR", TVec3f(76.33f, 15.69f, 88.99f), TVec3f(1.8f, 52.51f, 39.58f));
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
    _1D8 = new FBO[MR::getFrameBufferWidth()];
    _1DC = new FBO[MR::getFrameBufferWidth()];
    _1E4 = 0.0f;
    _1E8 = 0;
    _1EC = 0.0f;
    _F3C = new JAIAudible[0x1E];
    _F40 = 0;
    _F42 = 1;
    for (int i = 0; i < 0x1E; i++) {
        JAIAudible& rAudible = _F3C[i];
        rAudible._0 = 1.0f;
        rAudible._4 = 0.0f;
        rAudible._8 = 0.0f;
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
    _2C4 = _240.scaleInline(-70.0f);
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
    if (!mMario->_71C) {
        if (isAnimationRun("基本")) {
            mMario->changeAnimationUpper(pName, static_cast< const char* >(nullptr));
            return;
        }
    }
    mMario->changeAnimation(pName, static_cast< const char* >(nullptr));
}

void MarioActor::stopAnimation(const char* pName) {
    mMario->stopAnimation(pName, static_cast< const char* >(nullptr));
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
    if (mMario->isStatusActive(0x12)) {
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

    if (mMario->isAnimationRun("氷結") || mMario->isStatusActive(0xd)) {
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
    if (mMario->mMovementStates.jumping && !mMario->mMovementStates._1) {
        return true;
    }
    return false;
}

bool MarioActor::isJumpRising() const {
    bool ret = false;
    if (mMario->mMovementStates.jumping && mMario->isRising()) {
        ret = true;
    }
    return ret;
}

bool MarioActor::isPunching() const {
    if (mMario->isStatusActive(0x11)) {
        return true;
    }
    if (mMario->mSwim->mSpinTimer || mMario->mSwim->mSpinDashTimer) {
        return true;
    }
    if (_3E5) {
        return true;
    }
    if (!mMario->isAnimationTerminate(nullptr)) {
        if (mMario->isAnimationRun("空中ひねり")) {
            return true;
        }
        if (mMario->isAnimationRun("アイスひねり")) {
            return true;
        }
        if (mMario->isAnimationRun("アイスひねり空中")) {
            return true;
        }
        if (mMario->isAnimationRun("アイスひねり静止")) {
            return true;
        }
        if (mMario->isAnimationRun("アイスひねり移動")) {
            return true;
        }
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
    return mMario->isStatusActive(0x14) != false;
}

// does not want to cooperate
bool MarioActor::isSleeping() const {
    bool ret = true;
    if (!mMario->isAnimationRun("特殊ウエイト1B")) {
        if (!mMario->isAnimationRun("特殊ウエイト1A")) {
            ret = false;
        }
    }
    return ret;
}

bool MarioActor::isRefuseTalk() const {
    if (!isEnableNerveChange()) {
        return true;
    }
    if (mMario->isStatusActive(0x1C)) {
        return true;
    }
    if (mMario->isStatusActive(0x5)) {
        return true;
    }
    if (mMario->isStatusActive(0x13)) {
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
        if (mMario->mMovementStates.jumping) {
            return true;
        }
        if (!mMario->mMovementStates._1) {
            return true;
        }
        if (mMario->_3CE < 5) {
            return true;
        }
        if (mMario->mDrawStates._4) {
            return true;
        }
        if (mMario->_1C._15) {
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
    _46C = nullptr;
    _378++;
    _1E1 = 0;
    PSMTXCopy(_AE0.toMtxPtr(), _AB0.toMtxPtr());
    PSMTXCopy(MR::getCameraViewMtx(), _AE0.toMtxPtr());
    updateCameraInfo();
    _4A8 = 0;
    _4AC = 0.785398185253f;
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
            mPosition -= _288.scaleInline(0.5f);
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
                    mMario->mJumpVec += (-getGravityVec()).scaleInline(scale);
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
                    mMario->push(mMario->mFrontVec.scaleInline(5.0f));
                } else if (mMario->isAnimationRun("坂すべり上向きうつぶせ")) {
                    mMario->push(mMario->mFrontVec.scaleInline(-5.0f));
                }
                mMario->mDrawStates._2 = true;
            }
        }
        if (getMovementStates().jumping && !mAlphaEnable) {
            if (stack_128.dot(getGravityVec()) < -40.0f) {
                TVec3f stack_EC(mPosition - getGravityVec().scaleInline(100.0f));
                TVec3f stack_E0;
                Triangle* pTmp = mMario->getTmpPolygon();

                if (MR::getFirstPolyOnLineToMap(&stack_E0, pTmp, stack_EC, getGravityVec().scaleInline(200.f))) {
                    TVec3f stack_D4;
                    if (MR::vecKillElement(stack_E0 - mPosition, getGravityVec(), &stack_D4) < -5.0f && pTmp->mParts && !pTmp->mParts->_D4 &&
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
                mPosition -= mMario->_368.scaleInline(element);
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
    _F3C_vec[_F40] = mMario->mFrontVec;
    _F40 = (_F40 + 1) % _F42;
}

void MarioActor::control() {
    if (mSuperKinokoCollected) {
        if (MR::tryStartDemoWithoutCinemaFrame(this, "マリオスーパー化")) {  // 6-up camera
            mSuperKinokoCollected = false;
            changeMaxLife(6);
            MR::stopAnimFrame(this);
            MR::requestPowerUpHPMeter();
            mMario->startPadVib("マリオ[変身]");
            MR::startSystemSE("SE_SY_SUPER_KINOKO_GET", -1, -1);
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
            _424 = nullptr;
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
    _264 = mPosition - _270;
    _270 = mPosition;
    updateBindRatio();
    updateEffect();
    if (_B94 && !--_B94) {
        mMario->stopAnimationUpper("ハンマー投げ回転中", static_cast< const char* >(nullptr));
        mMario->stopAnimation("ハンマー投げ回転中", static_cast< const char* >(nullptr));
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
        mMario->stopAnimation(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
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
        } else if (!selectWaterInOut(_924->mHost->mName)) {
            s32 initial = mMario->mSwim->mSwimState;
            mMario->mSwim->checkWaterCube(false);
            if ((int)mMario->mSwim->mSwimState != initial) {
                if (mMario->mSwim->mSwimState <= MarioSwim::SWIM_STATE_ENTERING && static_cast<u32>(initial - 2) <= 1) {
                    playEffectRT("水面ジャンプ水柱", mMario->mSwim->mSurfacePos, mMario->mSwim->mSurfaceNorm);
                    emitEffectWaterColumn(mMario->mSwim->mSurfacePos, mMario->mSwim->mSurfaceNorm);
                    // SWIM_STATE_UNDERWATER and SWIM_STATE_SURFACE
                } else if (static_cast<u32>(initial) <= 1 && mMario->mSwim->mSwimState - MarioSwim::SWIM_STATE_UNDERWATER <= 1) {
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
            mMario->startPadVib(static_cast< u32 >(0));
            mMario->playSound("スピン回復終了", -1);
            Color8 stack_8;
            stack_8.set(0x50, 0x80, 0xc8, 0);
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
    if (isJumping() && mPlayerMode != 6 && !mMario->isStatusActive(0x18)) {
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
    if (mMario->isStatusActive(0x18)) {
        canRush = false;
    }
    if (mMario->isStatusActive(0x13)) {
        canRush = false;
    }
    if (_468) {
        canRush = false;
    }
    if (mMario->isStatusActive(2)) {
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
                mMario->changeAnimation("ハチスピン空中", static_cast< const char* >(nullptr));
            } else if (getMovementStates()._A || mAlphaEnable) {
                mMario->changeAnimation("サマーソルト", static_cast< const char* >(nullptr));
            } else {
                mMario->changeAnimation("ハチスピン", static_cast< const char* >(nullptr));
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
            mMario->changeAnimation("ハチスピン空中", static_cast< const char* >(nullptr));
        } else {
            if (getMovementStates()._A || mAlphaEnable) {
                mMario->changeAnimation("サマーソルト", static_cast< const char* >(nullptr));  // Summersault
            } else {
                mMario->changeAnimation("ハチスピン", static_cast< const char* >(nullptr));
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
    if (mMario->isStatusActive(0x12)) {
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
                mMario->changeAnimation("つぶれ解除", static_cast< const char* >(nullptr));
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
            mMario->changeAnimation("つぶれ解除", static_cast< const char* >(nullptr));
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
    const TRot3f* pMtx = reinterpret_cast<const TRot3f*>(rawMtx);
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
    i += _3B4.scaleInline(elementX);
    j += _3B4.scaleInline(elementY);
    k += _3B4.scaleInline(elementZ);
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

void MarioActor::forceSetBaseMtx(MtxPtr mtx) {
    _EA5 = true;
    _1C0 = true;
    PSMTXCopy(mtx, _EA8);
    MR::extractMtxTrans(mtx, &_2F4);
    if (_482) {
        MR::extractMtxTrans(mtx, &mPosition);
    }
    const TRot3f* pMtx = reinterpret_cast<const TRot3f*>(mtx);
    pMtx->getZDir(_2DC);
    pMtx->getYDir(_2D0);
    pMtx->getXDir(_2E8);
    MR::updateHitSensorsAll(this);
    mMario->invalidateRelativePosition();
    mMario->_8F8.zero();
    mMario->mMovementStates._23 = 0;
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

XanimeResourceTable* MarioActor::getResourceTable() const {
    return mMarioAnim->mResourceTable;
}

void MarioActor::setPunchHitTimer(u8 punchHitTime) {
    if (_944 == 0) {
        _945 = 0;
        _974 = 0;
    }

    _944 = punchHitTime;
}

void MarioActor::getRealPos(const char* pName, TVec3f* pPos) const {
    TMtx34f mtx;
    getRealMtx(mtx.toMtxPtr(), pName);
    MR::extractMtxTrans(mtx.toMtxPtr(), pPos);
}

MtxPtr MarioActor::getGlobalJointMtx(const char* pName) {
    const u16 jointIndex = MR::getJointIndex(this, pName);
    TMtx34f* jointMtx = static_cast< TMtx34f* >(_C28);
    getRealMtx(jointMtx[jointIndex].toMtxPtr(), pName);
    return jointMtx[jointIndex].toMtxPtr();
}

void MarioActor::calcAnimInMovement() {
    if (mHealth != 0) {
        if (mMario->isDamaging() || mMario->getDamageAfterTimer() != 0) {
            _1AA = 0;
            _1A8++;
        }
    } else if (_1AA != 0) {
        _1AA--;
    }

    _1C0 = true;
    updateBlink();
    calcSpinEffect();

    if (mCurrModel == 4) {
        _9C8->calcAnim();
        _A50->calcAnim();
        _A54->calcAnim();

        f32 brkFrame = MR::getBrkFrame(_9C8);
        MR::setBrkFrame(_A50, brkFrame);
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
        if (mPlayerMode == 6) {
            const MarioConstTable* pTable = mConst->getTable();
            if (mMario->mVerticalSpeed < pTable->mTeresaWaitHeight) {
                mPosition -= getGravityVec();
            }
        }

        bool isFirst = true;
        switch (mPlayerMode) {
        case 4:
            isFirst = MR::isOnGameEventFlagBeeMarioAtFirst();
            break;
        case 6:
            isFirst = MR::isOnGameEventFlagTeresaMarioAtFirst();
            break;
        case 5:
            isFirst = MR::isOnGameEventFlagHopperMarioAtFirst();
            break;
        case 2:
            isFirst = MR::isOnGameEventFlagFireMarioAtFirst();
            break;
        case 3:
            isFirst = MR::isOnGameEventFlagIceMarioAtFirst();
            break;
        case 7:
            isFirst = MR::isOnGameEventFlagFlyingMarioAtFirst();
            break;
        default:
            break;
        }

        if (!isFirst) {
            const LiveActor* pTarget = _4A4->mActor;
            TVec3f front;
            MR::calcFrontVec(&front, pTarget);
            mMario->setFrontVecKeepUp(front);

            if (_336 == 0) {
                MR::zoomInTargetGameCamera();
                CameraTargetArg arg(nullptr, nullptr, nullptr, this);
                MR::startGlobalEventCamera("変身初出カメラ", arg, 0x3C);

                TVec3f targetPos(_2A0);
                f32 cameraDist = 500.0f;
                if (mPlayerMode == 6) {
                    cameraDist = 700.0f;
                }

                const Mario::MovementStates& states = mMario->getMovementStates();
                if (states._B && !states._1) {
                    targetPos += mMario->mJumpVec;
                }

                TVec3f camOffset(front);
                camOffset.scale(cameraDist);
                TVec3f cameraPos(targetPos);
                cameraPos += camOffset;
                TVec3f up = -_240;
                MR::setProgrammableCameraParam("変身初出カメラ", targetPos, cameraPos, up, true);
            }

            _336 = 1;
        }

        _3D8--;
        if (_3D8 == 0) {
            if (mTransforming) {
                MR::endDemo(this, "マリオ変身");
                mTransforming = false;
            }

            MR::releaseAnimFrame(this);
            _3DE = 1;

            switch (mPlayerMode) {
            case 4:
                MR::explainBeeMarioIfAtFirst();
                break;
            case 6:
                MR::explainTeresaMarioIfAtFirst();
                break;
            case 5:
                MR::explainHopperMarioIfAtFirst();
                break;
            case 2:
                MR::explainFireMarioIfAtFirst();
                break;
            case 3:
                MR::explainIceMarioIfAtFirst();
                break;
            case 7:
                MR::explainFlyingMarioIfAtFirst();
                break;
            case 1:
                MR::explainInvincibleMarioIfAtFirst();
                break;
            default:
                break;
            }

            if (_336 != 0) {
                _338 = 0xA;
            }
            _336 = 0;
        }

        _3DF = 1;
    } else {
        mMario->updateMorphResetTimer();
    }

    if (_338 != 0) {
        _338--;
        if (_338 == 0) {
            MR::zoomOutTargetGameCamera();
            MR::endGlobalEventCamera("変身初出カメラ", 0x3C, true);
        }
    }
}

void MarioActor::calcAnim() {
    if (_482) {
        mBlendMtxTimer = 0;
        return;
    }

    if (_3DF) {
        u16 mode = (_3D8 & 7) < (_3D8 >> 3) ? _3D6 : mPlayerMode;

        if (_3D8 != 0 && mPlayerMode == 0) {
            if (mode != mPlayerMode && _3D6 == 6) {
                if (_3D8 > 0x1A) {
                    _9A4->appear();
                }

                _483 = true;
                updateHand();
                updateFace();
                return;
            }

            if (_3D6 == 6) {
                _9A4->kill();
                _483 = false;
            }
        }

        switch (mode) {
        case 1:
            changeDisplayMode(4);
            _9C8->calcAnim();
            if (_3DE) {
                _A6E = true;
            }
            break;
        case 3:
            changeDisplayMode(3);
            break;
        case 4:
        case 2:
            changeDisplayMode(2);
            break;
        case 5:
            _3DF = false;
            changeDisplayMode(5);
            break;
        default:
            changeDisplayMode(0);
            s32 btpFrame = 0;
            if (mode == 7) {
                btpFrame = 1;
            } else if (mode == 2) {
                btpFrame = 2;
            }

            MR::startBtp(this, "ColorChange");
            MR::setBtpFrameAndStop(this, static_cast< f32 >(btpFrame));
            break;
        }

        if (_3DF) {
            changeHandMaterial();
        }
    }

    if (mMario->isStatusActive(0xB)) {
        if (mCurrModel != 1) {
            _A0B = mCurrModel;
        }

        if ((_37C & 0x3) != 0) {
            changeDisplayMode(1);
        } else {
            changeDisplayMode(_A0B);
        }
    } else if (mCurrModel == 1 && mHealth != 0) {
        changeDisplayMode(_A0B);
    }

    _3DF = false;
    _3DE = false;

    calcAndSetBaseMtx();
    mMarioAnim->switchMirrorMode();
    mMarioAnim->calc();

    if (_3D8 == 0) {
        switch (mPlayerMode) {
        case 3:
            _9C0->mPosition = mPosition;
            MR::updateModelDiffDL(_9C0);
            MR::updateModelDiffDL(_A40);
            MR::updateModelDiffDL(_A44);
            break;
        case 1:
            _9C8->mPosition = mPosition;
            MR::updateModelDiffDL(_9C8);
            MR::updateModelDiffDL(_A50);
            MR::updateModelDiffDL(_A54);
            break;
        case 5:
            _A00->mPosition = mPosition;
            MR::updateModelDiffDL(_A00);
            break;
        default:
            if (mCurrModel != 1) {
                MR::updateModelDiffDL(this);
            } else {
                _9E4->mPosition = mPosition;
                MR::updateModelDiffDL(_9E4);
            }
            break;
        }
    } else if (mCurrModel == 0) {
        MR::updateModelDiffDL(this);
    }

    if (MR::isHiddenModel(this) && !_482) {
        MR::showModel(this);
    }

    updateRealMtx();

    if (!(mMario->mMovementStates._F && mMario->_544 > 1)) {
        _214->setMode(mAlphaEnable ? 0 : 2);

        TVec3f shadowDiff(mMario->mShadowPos);
        shadowDiff -= mPosition;
        _214->_20 = 40.0f + shadowDiff.dot(_240);
        _214->_48 = mMario->mShadowPos;

        TMtx34f handMtx;
        getRealMtx(handMtx.toMtxPtr(), "HandL");
        TVec3f handPos;
        MR::extractMtxTrans(handMtx.toMtxPtr(), &handPos);

        TMtx34f footMtx;
        getRealMtx(footMtx.toMtxPtr(), "FootR");
        TVec3f footPos;
        MR::extractMtxTrans(footMtx.toMtxPtr(), &footPos);

        TVec3f side(footPos);
        side -= handPos;
        f32 sideLen = side.length();
        MR::normalizeOrZero(&side);

        TVec3f tangent;
        side.cross(_240, tangent);
        MR::normalizeOrZero(&tangent);
        _240.cross(tangent, side);
        if (MR::normalizeOrZero(&side)) {
            mMario->mSideVec = side;
        }

        _214->_3C = side;
        if (mMario->isSwimming()) {
            _214->_18 = 10.0f + sideLen;
            _214->_1C = 140.0f;
        } else {
            _214->_18 = 5.0f + sideLen;
            _214->_1C = 70.0f;
        }

        TMtx34f centerMtx;
        if (mMario->isStatusActive(5)) {
            getRealMtx(centerMtx.toMtxPtr(), "Center");
        } else {
            getRealMtx(centerMtx.toMtxPtr(), "All_Root");
        }

        TVec3f centerPos;
        MR::extractMtxTrans(centerMtx.toMtxPtr(), &centerPos);
        _214->create(centerPos, _240, mMario->mFrontVec);
    }

    if (!_1C1) {
        mMario->updateLookOfs();
    }

    updateHand();
    updateFace();
    calcScreenBoxRange();

    if (_946 != 0 || _F20 != 0 || _F21 != 0 || mMario->mMovementStates._2B) {
        _94A = 0;
    } else {
        _94A++;
    }

    if (mCurrModel != 0) {
        J3DModelX* baseModel = mModels[0];
        J3DModelX* drawModel = getJ3DModel();

        MtxPtr handLMtx = MR::getJointMtx(baseModel, "HandL0");
        PSMTXCopy(MR::getJointMtx(drawModel, "HandL0"), handLMtx);

        MtxPtr handRMtx = MR::getJointMtx(baseModel, "HandR0");
        PSMTXCopy(MR::getJointMtx(drawModel, "HandR0"), handRMtx);

        MtxPtr footLMtx = MR::getJointMtx(baseModel, "FootL");
        PSMTXCopy(MR::getJointMtx(drawModel, "FootL"), footLMtx);

        MtxPtr footRMtx = MR::getJointMtx(baseModel, "FootR");
        PSMTXCopy(MR::getJointMtx(drawModel, "FootR"), footRMtx);

        MtxPtr centerJoint = MR::getJointMtx(baseModel, "Center");
        PSMTXCopy(MR::getJointMtx(drawModel, "Center"), centerJoint);

        MtxPtr allRootJoint = MR::getJointMtx(baseModel, "All_Root");
        PSMTXCopy(MR::getJointMtx(drawModel, "All_Root"), allRootJoint);

        MtxPtr emptyJoint = MR::getJointMtx(baseModel, "");
        PSMTXCopy(MR::getJointMtx(drawModel, ""), emptyJoint);

        MtxPtr hipJoint = MR::getJointMtx(baseModel, "Hip");
        PSMTXCopy(MR::getJointMtx(drawModel, "Hip"), hipJoint);

        MtxPtr faceMtx = MR::getJointMtx(baseModel, "Face0");
        PSMTXCopy(MR::getJointMtx(drawModel, "Face0"), faceMtx);

        PSMTXCopy(drawModel->getBaseTRMtx(), baseModel->getBaseTRMtx());
    }

    updateRasterScroll();
}

void MarioActor::calcAndSetBaseMtx() {
    if (!_1C0) {
        _1C1 = true;
        return;
    }

    _1C1 = false;
    _1C0 = false;

    TMtx34f prevBlendMtx;
    PSMTXCopy(getJ3DModel()->getBaseTRMtx(), prevBlendMtx.toMtxPtr());

    bool hasForcedBaseMtx = false;

    if (_934) {
        HitSensor* sensor = getSensor("body");
        hasForcedBaseMtx = _924->receiveMessage(0xA1, sensor);

        if (hasForcedBaseMtx) {
            TVec3f head;
            const TRot3f* pMtx = reinterpret_cast<const TRot3f*>(getBaseMtx());
            pMtx->getYDir(head);
            MR::normalizeOrZero(&head);
            mMario->setHeadVec(head);

            TVec3f front;
            pMtx->getZDir(front);
            MR::normalizeOrZero(&front);
            mMario->setFrontVecKeepUp(front);
            mMario->_334 = front;
        }
    }

    if (_EA4 || _EA5) {
        if (_EA5) {
            PSMTXCopy(getBaseMtx(), _EA8.toMtxPtr());

            TVec3f head;
            const TRot3f* pMtx = reinterpret_cast<const TRot3f*>(getBaseMtx());
            pMtx->getYDir(head);
            mMario->setHeadVec(head);

            TVec3f front;
            pMtx->getZDir(front);
            mMario->setFrontVecKeepUp(front);

            mMario->_278 = 0.0f;
            mMario->_71C = false;
            mMario->stopJump();
            mMario->_334 = mMario->mFrontVec;

            if (mMario->isSwimming()) {
                mMario->mSwim->resetAndFixPose();
            }

            hasForcedBaseMtx = true;
            if (_EA4) {
                _EA6 = true;
            }
        } else if (_EA6) {
            return;
        }
    }

    TMtx34f prevBaseMtx;

    if (hasForcedBaseMtx) {
        TVec3f prevPos(mPosition);
        MR::extractMtxTrans(getBaseMtx(), &mPosition);
        mMario->invalidateRelativePosition();

        TVec3f forceDelta(mPosition);
        forceDelta -= prevPos;
        _938 = forceDelta;

        PSMTXCopy(getBaseMtx(), prevBaseMtx.toMtxPtr());

        if (mMario->mMovementStates._37) {
            mMario->_688 = mPosition;
        }

        updateGravityVec(true, true);
        mMario->updateGroundInfo();
    }

    if (_934 && !hasForcedBaseMtx) {
        TVec3f basePos;
        MR::extractMtxTrans(getBaseMtx(), &basePos);

        TVec3f forceDelta(mPosition);
        forceDelta -= basePos;
        _938 = forceDelta;
    }

    TPos3f baseMtx;
    PSMTXIdentity(baseMtx.toMtxPtr());
    mMario->createAngleMtx(baseMtx.toMtxPtr(), hasForcedBaseMtx);

    if (_EA6) {
        TMtx34f capMtx;
        getRealMtx(capMtx.toMtxPtr(), "CapPosition");
        MR::extractMtxTrans(capMtx.toMtxPtr(), &_2AC);

        TVec3f capDiff(_2AC);
        capDiff -= mPosition;
        if (capDiff.length() > 500.0f) {
            PSMTXCopy(getBaseMtx(), baseMtx.toMtxPtr());
            MR::setMtxTrans(baseMtx.toMtxPtr(), 0.0f, 0.0f, 0.0f);
        }
    }

    PSMTXConcat(baseMtx.toMtxPtr(), MR::tmpMtxRotYRad(mMario->mYAngleOffset), baseMtx.toMtxPtr());

    if (mPlayerMode == 6) {
        updateBaseMtxTeresa(baseMtx.toMtxPtr());
    }

    if (!hasForcedBaseMtx && mPlayerMode == 4 && !mMario->isStatusActive(0x16) && !mMario->isStatusActive(0x15) && !mMario->isStatusActive(0x1B) &&
        !mMario->mMovementStates._23 && !mMario->mMovementStates._A && !(mMario->mMovementStates._B && mMario->mMovementStates.jumping)) {
        _9F4 = mMario->mAirGravityVec;

        const MarioConstTable* table = mConst->getTable();
        f32 headToFootLength = table->mBeePoseHeadToFootLength;

        TVec3f antiGravity = -getGravityVector();
        TVec3f headOffset(antiGravity);
        headOffset.scale(headToFootLength);

        TVec3f headPivot = -headOffset;
        MtxPtr transMtx = MR::tmpMtxTrans(headPivot);

        TVec3f delayOffset(_9F4);
        delayOffset.scale(headToFootLength);

        TVec3f targetPos(mPosition);
        targetPos -= delayOffset;

        TVec3f delayDir(_33C);
        delayDir -= targetPos;

        f32 delayAngle = 0.0f;
        TVec3f delayAxis;
        if (!MR::normalizeOrZero(&delayDir)) {
            mMario->mFrontVec.cross(delayDir, delayAxis);
            if (!MR::normalizeOrZero(&delayAxis)) {
                delayAngle = MR::acosEx(_9F4.dot(delayDir));
                f32 limit = table->mBeePoseDelayAngleAir;

                if (mMario->mMovementStates._1) {
                    limit = table->mBeePoseDelayAngleGround;
                }

                delayAngle = MR::clamp(delayAngle, 0.0f, limit);
            }
        }

        TMtx34f delayRotMtx;
        PSMTXRotAxisRad(delayRotMtx.toMtxPtr(), &delayAxis, delayAngle);

        PSMTXConcat(transMtx, baseMtx.toMtxPtr(), baseMtx.toMtxPtr());
        PSMTXConcat(delayRotMtx.toMtxPtr(), baseMtx.toMtxPtr(), baseMtx.toMtxPtr());
        PSMTXConcat(MR::tmpMtxTrans(headOffset), baseMtx.toMtxPtr(), baseMtx.toMtxPtr());

        TVec3f delayAccel(_9F4);
        delayAccel.scale(table->mBeePoseDelayAccel);
        MR::vecKillElement(delayAccel, _360, &delayAccel);

        _354 += delayAccel;
        _33C += _354;

        TVec3f offsetFromHead(_9F4);
        offsetFromHead.scale(headToFootLength);

        TVec3f poseBasePos(mPosition);
        poseBasePos -= offsetFromHead;

        TVec3f poseVec(_33C);
        poseVec -= poseBasePos;
        _360 = poseVec;

        TVec3f prevPose(_360);
        TVec3f normPose(_360);
        MR::normalizeOrZero(&normPose);

        f32 poseAngle = MR::acosEx(_9F4.dot(normPose));
        f32 poseLimit = table->mBeePoseLimitAngleAir;

        if (mMario->mMovementStates._1) {
            poseLimit = table->mBeePoseLimitAngleGround;
        }

        if (poseAngle > poseLimit) {
            TVec3f poseAxis;
            _9F4.cross(normPose, poseAxis);

            if (MR::normalizeOrZero(&poseAxis)) {
                _360 = _9F4;
            } else {
                TMtx34f poseRotMtx;
                PSMTXRotAxisRad(poseRotMtx.toMtxPtr(), &poseAxis, poseLimit);
                PSMTXMultVec(poseRotMtx.toMtxPtr(), &_9F4, &_360);
            }
        }

        _360.setLength(headToFootLength);

        if (prevPose.length() > headToFootLength) {
            TVec3f poseRest(prevPose);
            poseRest -= _360;
            _354 += -poseRest;
        }

        TVec3f followOffset(_9F4);
        followOffset.scale(headToFootLength);

        TVec3f followPos(mPosition);
        followPos -= followOffset;
        followPos += _360;
        _33C = followPos;

        MR::normalizeOrZero(&_360);

        if (_354.length() > table->mBeePoseDelaySpeedLimit) {
            _354.setLength(table->mBeePoseDelaySpeedLimit);
        }

        f32 friction = table->mBeePoseFrictionStop;
        if (!mMario->_71C && !isJumping()) {
            friction = table->mBeePoseFrictionMove;
        }
        _354.scale(friction);

        if (!MR::isSameDirection(_360, mMario->mFrontVec, 0.01f)) {
            TVec3f up(_360 - mMario->mFrontVec);
            MR::makeMtxUpFront(&baseMtx, up, mMario->mFrontVec);
        } else if (!MR::isSameDirection(mMario->_1FC, mMario->mFrontVec, 0.01f)) {
            MR::makeMtxUpFront(&baseMtx, mMario->_1FC, mMario->mFrontVec);
        } else {
            MR::makeMtxUpFront(&baseMtx, mMario->mHeadVec, mMario->mFrontVec);
        }

        if (mMario->_71C > 2 || isJumping()) {
            f32 keepRatio = table->mBeePoseTransBlendingRatioMove;
            f32 blendRatio = 1.0f - keepRatio;

            TVec3f blendPos(_33C);
            blendPos -= mPosition;
            blendPos.scale(blendRatio);

            TVec3f keepPos(_348);
            keepPos.scale(keepRatio);
            keepPos += blendPos;
            _348 = keepPos;

            MR::setMtxTrans(baseMtx.toMtxPtr(), _348.x, _348.y, _348.z);
        } else {
            _348.scale(table->mBeePoseTransBlendingRatioStop);
            MR::setMtxTrans(baseMtx.toMtxPtr(), _348.x, _348.y, _348.z);
        }
    } else {
        _354.zero();
        _348.zero();
        _33C = mPosition;
        _9F4 = getGravityVector();
    }

    MR::addTransMtx(baseMtx.toMtxPtr(), mPosition);

    if (hasForcedBaseMtx) {
        TMtx34f invBaseMtx;
        PSMTXInverse(baseMtx.toMtxPtr(), invBaseMtx.toMtxPtr());
        PSMTXConcat(invBaseMtx.toMtxPtr(), prevBaseMtx.toMtxPtr(), _E3C.toMtxPtr());
        mMarioAnim->mXanimePlayer->mCore->getJointTransform(0)->_64 = _E3C.toMtxPtr();
    } else {
        TMtx34f correctionMtx;
        TVec3f correctionTrans;
        mMario->createCorrectionMtx(correctionMtx.toMtxPtr(), &correctionTrans);

        XjointTransform* joint1 = mMarioAnim->mXanimePlayer->mCore->getJointTransform(1);
        joint1->_2C = correctionTrans.x;
        joint1->_30 = correctionTrans.y;
        joint1->_34 = correctionTrans.z;

        PSMTXCopy(correctionMtx.toMtxPtr(), _E3C.toMtxPtr());
        mMarioAnim->mXanimePlayer->mCore->getJointTransform(0)->_64 = _E3C.toMtxPtr();
    }

    if (_390 != 0) {
        _394 = 30;

        switch (_39C) {
        case 0:
        case 2:
            if (_390 > 0xF) {
                f32 distToCeil = mMario->calcDistToCeil(false);
                f32 distToPress = mMario->calcDistToCeilOnPress();

                f32 targetScale = MR::clamp(distToCeil / 150.0f, 0.2f, 1.0f);
                if (_3B0 > targetScale) {
                    _3B0 = targetScale;
                } else {
                    targetScale = MR::clamp(distToPress / 150.0f, 0.2f, 1.0f);
                    if (_3B0 > targetScale) {
                        _3B0 = targetScale;
                    }
                }

                MR::setMtxTrans(baseMtx.toMtxPtr(), mPosition.x, mPosition.y, mPosition.z);

                if (mMario->_960 != 0x1B) {
                    _1E0 = true;
                }
            } else {
                _3B0 = 0.2f + ((0.8f * static_cast< f32 >(0xF - _390)) / 15.0f);
            }

            if (_398 != 0) {
                _3B0 += 0.02f * static_cast< f32 >(_398);
            }
            break;
        case 1:
        case 3:
            if (_390 > 0xF) {
                f32 distWidth = mMario->calcDistWidth();
                mPosition = mMario->mPosition;

                f32 targetScale = MR::clamp(distWidth / 80.0f, 0.2f, 1.0f);
                if (_3B0 > targetScale) {
                    _3B0 = targetScale;
                } else {
                    _3B0 = MR::clamp(_3B0 - 0.01f, 0.2f, 1.0f);
                }
            } else {
                _3B0 = 0.2f + ((0.8f * static_cast< f32 >(0xF - _390)) / 15.0f);
            }

            if (_398 != 0) {
                _3B0 += 0.02f * static_cast< f32 >(_398);
            }
            break;
        case 4: {
            f32 angle = (static_cast< f32 >(_390) / 120.0f) * (0.5f * 3.1415927f);
            if (angle < 0.0f) {
                angle = -angle;
            }
            _3B0 = 1.0f - JMACosRadian(angle);
            break;
        }
        default:
            break;
        }
    } else if (_394 != 0) {
        _394--;

        switch (_39C) {
        case 0:
        case 2: {
            f32 phase = (2.0f * 3.1415927f * static_cast< f32 >(_394)) / 15.0f;
            f32 wave = MR::sin(phase);
            _3B0 = 1.0f + (((0.15f * static_cast< f32 >(_394)) / 30.0f) * wave);
            break;
        }
        case 1:
        case 3: {
            f32 phase = (2.0f * 3.1415927f * static_cast< f32 >(_394)) / 15.0f;
            f32 wave = MR::sin(phase);
            _3B0 = 1.0f + (((0.15f * static_cast< f32 >(_394)) / 30.0f) * wave);
            break;
        }
        default:
            break;
        }
    } else if (!mMario->mMovementStates._3C) {
        _3B0 = 1.0f;
    }

    if (_398 != 0) {
        _398--;
    }

    if (mBlendMtxTimer != 0) {
        f32 total = static_cast< f32 >(mBlendMtxTimer);
        f32 remain = static_cast< f32 >(mBlendMtxTimer - 1);
        _EA0 = 1.0f - ((1.0f - _EA0) * (remain / total));
        MR::blendMtx(prevBlendMtx.toMtxPtr(), baseMtx.toMtxPtr(), _EA0, baseMtx.toMtxPtr());
        mBlendMtxTimer--;
    }

    PSMTXCopy(baseMtx.toMtxPtr(), _3EC.toMtxPtr());

    if (_3B0 != 1.0f) {
        scaleMtx(baseMtx.toMtxPtr());
    }

    J3DModelX* model = getJ3DModel();
    PSMTXCopy(baseMtx.toMtxPtr(), model->getBaseTRMtx());
    model->mBaseScale.x = mScale.x;
    model->mBaseScale.y = mScale.y;
    model->mBaseScale.z = mScale.z;
    _EA5 = false;
}

void MarioActor::jumpHop() {
    if (!isJumping()) {
        return;
    }

    if (!mMario->isRising()) {
        mMario->cutGravityElementFromJumpVec(true);

        TVec3f antiGravity(-_240);
        antiGravity.scale(mConst->getTable()->mClapJumpBonusFalling);
        mMario->mJumpVec += antiGravity;
    } else {
        f32 speed = -mMario->cutGravityElementFromJumpVec(true);
        const MarioConstTable* table = mConst->getTable();
        if (speed < table->mHopLimit) {
            speed += table->mClapJumpBonusRising;
            if (speed > table->mHopLimit) {
                speed = table->mHopLimit;
            }
        }

        TVec3f antiGravity(-_240);
        antiGravity.scale(speed);
        mMario->mJumpVec += antiGravity;
    }

    if (mMario->_430 == 5) {
        mMario->_430 = 0;
        mMario->mMovementStates_LOW_WORD &= ~0x00100000;
        mMario->changeAnimation("落下", static_cast< const char* >(nullptr));
    }

    if (mMario->_430 == 0xB) {
        mMario->_430 = 0;
    }

    mMario->_4B0 = mMario->mPosition;
}

void MarioActor::calcCenterPos() {
    if (_934 || _8C) {
        MR::copyJointPos(this, "Spine1", &_2A0);
        return;
    }

    TVec3f up;
    if (mMario->isStatusActive(1)) {
        up = mMario->_75C;
    } else if (mMario->isStatusActive(5)) {
        MR::copyJointPos(this, "Spine1", &_2A0);
        return;
    } else if (mMario->isSwimming()) {
        up = _4B8;
    } else {
        up = -getGravityVec();
    }

    if (mMario->mMovementStates._15) {
        mBinder->mRadius = 0.8f * mBinder->mRadius + 0.2f * 40.0f;
    } else {
        mBinder->mRadius = 0.9f * mBinder->mRadius + 0.1f * 60.0f;
    }

    f32 centerDist = mBinder->mRadius;
    if (mPlayerMode == 6) {
        f32 dist = 0.8f * mBinder->mRadius + 0.2f * 100.0f;
        centerDist = dist + 180.0f;
        mBinder->mRadius = dist;
    }

    up.scale(centerDist);

    TVec3f center(mPosition);
    center += up;
    _2A0 = center;
}

void MarioActor::calcHeadPos() {
    TMtx34f mtx;
    getRealMtx(mtx.toMtxPtr(), "CapPosition");
    MR::extractMtxTrans(mtx.toMtxPtr(), &_2AC);
}

void MarioActor::setPress(u8 pressType, s32 pressTimer) {
    if (_390 != 0) {
        return;
    }

    if (pressType == 0 && mMario->calcDistToCeil(false) >= 160.0f) {
        return;
    }

    if (mMario->_10._1D) {
        return;
    }

    if (_39C == 0 || _39C == 2) {
        *mMario->_480 = *mMario->mGroundPolygon;
        *mMario->_484 = *mMario->_4C8;
    }

    if (mMario->isStatusActive(4)) {
        mMario->closeStatus(nullptr);
    }

    mMario->startPadVib("マリオ[つぶれ]");
    MR::forceDeleteEffectAll(this);
    mVelocity.zero();

    if (pressTimer == 0) {
        pressTimer = 0xF0;
    }

    _390 = pressTimer;
    mMario->mMovementStates_LOW_WORD &= ~0x00100000;
    mMario->mMovementStates_LOW_WORD &= ~0x00010000;
    mMario->stopJump();
    mMario->mMovementStates_LOW_WORD |= 0x40000000;

    mMario->_13C.zero();
    mMario->_148.zero();
    mMario->_154.zero();
    mMario->mVerticalSpeed = 0.0f;

    mMario->stopWalk();
    mMarioAnim->setSpeed(0.0f);

    _B90 = true;
    _39C = pressType;
    _F44 = false;

    mMario->playSound("プレスダメージ", -1);
    mMario->playSound("声大ダメージ", -1);

    _FB4 = &NrvMarioActor::MarioActorNrvGameOver::sInstance;
    _FB8 = 0x3C;

    if (_39C != 4) {
        _390 = 1000;
    }

    for (u32 i = 0; i < 6; i++) {
        decLife(0);
    }

    MR::startStarPointerModeDemoMarioDeath(this);
}

bool MarioActor::isEnableMoveMario() const {
    if (isNerve(&NrvMarioActor::MarioActorNrvWait::sInstance)) {
        return true;
    }
    if (isNerve(&NrvMarioActor::MarioActorNrvNoRush::sInstance)) {
        return true;
    }
    return isNerve(&NrvMarioActor::MarioActorNrvGameOverNonStop::sInstance);
}

bool MarioActor::isEnableNerveChange() const {
    if (isNerve(&NrvMarioActor::MarioActorNrvWait::sInstance)) {
        return true;
    }
    return isNerve(&NrvMarioActor::MarioActorNrvNoRush::sInstance);
}

void MarioActor::forceGameOver() {
    if (isEnableNerveChange()) {
        setNerve(&NrvMarioActor::MarioActorNrvGameOver::sInstance);
    }
}

void MarioActor::forceGameOverAbyss() {
    if (isEnableNerveChange()) {
        setNerve(&NrvMarioActor::MarioActorNrvGameOverAbyss::sInstance);
    }
}

void MarioActor::forceGameOverBlackHole() {
    if (isEnableNerveChange()) {
        setNerve(&NrvMarioActor::MarioActorNrvGameOverBlackHole::sInstance);
    }
}

void MarioActor::forceGameOverNonStop() {
    if (isEnableNerveChange()) {
        setNerve(&NrvMarioActor::MarioActorNrvGameOverNonStop::sInstance);
    }
}

void MarioActor::forceGameOverSink() {
    if (isEnableNerveChange()) {
        setNerve(&NrvMarioActor::MarioActorNrvGameOverSink::sInstance);
    }
}

void MarioActor::updateCameraInfo() {
    _F74 = false;

    TVec3f camZDir = MR::getCamZdir();
    if (MR::diffAngleAbs(mCamDirZ, camZDir) >= 0.5235988f) {
        _F74 = true;
    }

    TVec3f camPos = MR::getCamPos();
    TVec3f moveDist(mCamPos);
    moveDist -= camPos;

    if (moveDist.length() > 500.0f) {
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
    if (_F48 && pTriangle->mSensor == _F48) {
        return true;
    }
    return false;
}

namespace NrvMarioActor {
INIT_NERVE(MarioActorNrvWait);
INIT_NERVE(MarioActorNrvGameOver);
INIT_NERVE(MarioActorNrvGameOverAbyss);
INIT_NERVE(MarioActorNrvGameOverAbyss2);
INIT_NERVE(MarioActorNrvGameOverFire);
INIT_NERVE(MarioActorNrvGameOverBlackHole);
INIT_NERVE(MarioActorNrvGameOverNonStop);
INIT_NERVE(MarioActorNrvGameOverSink);
INIT_NERVE(MarioActorNrvTimeWait);
INIT_NERVE(MarioActorNrvNoRush);
}  // namespace NrvMarioActor

namespace MR {
    s32 getFrameBufferWidth() {
        return JUTVideo::getManager()->getRenderMode()->fbWidth;
    }
}

namespace JGeometry {
    template <>
    void TRotation3< TMatrix34< SMatrix34C< f32 > > >::getEuler(TVec3f& rDest) const {
        if (this->mMtx[2][0] - 1.0f >= -0.0000038146973f) {
            rDest.x = ::JMath::sAtanTable.atan2_(-this->mMtx[0][1], this->mMtx[1][1]);
            rDest.y = -1.5707964f;
            rDest.z = 0.0f;
        } else if (this->mMtx[2][0] + 1.0f <= 0.0000038146973f) {
            rDest.x = ::JMath::sAtanTable.atan2_(this->mMtx[0][1], this->mMtx[1][1]);
            rDest.y = 1.5707964f;
            rDest.z = 0.0f;
        } else {
            rDest.x = ::JMath::sAtanTable.atan2_(this->mMtx[2][1], this->mMtx[2][2]);
            rDest.z = ::JMath::sAtanTable.atan2_(this->mMtx[1][0], this->mMtx[0][0]);
            rDest.y = JGeometry::TUtil< f32 >::asin(-this->mMtx[2][0]);
        }
    }
}

template <>
bool TriangleFilterDelegator< MarioActor >::isInvalidTriangle(const Triangle* pTriangle) const {
    return (mParent->*mFunc)(pTriangle);
}
