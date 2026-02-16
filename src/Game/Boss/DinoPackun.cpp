#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunBall.hpp"
#include "Game/Boss/DinoPackunDemoPosition.hpp"
#include "Game/Boss/DinoPackunEggShell.hpp"
#include "Game/Boss/DinoPackunSequencer.hpp"
#include "Game/Boss/DinoPackunTail.hpp"
#include "Game/Boss/DinoPackunTailNode.hpp"
#include "Game/Boss/DinoPackunTailPart.hpp"
#include "Game/Boss/DinoPackunTailRoot.hpp"
#include "Game/Boss/DinoPackunVs1.hpp"
#include "Game/Boss/DinoPackunVs2.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util/FootPrint.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

namespace {
    static TVec3f sHeadHitOffset = TVec3f(140.0f, -110.0f, 0.0f);
    static TVec3f sBodyHitOffset = TVec3f(0.0f, 200.0f, 0.0f);
    static TVec3f sEggHitOffset = TVec3f(0.0f, 200.0f, 0.0f);
    static TVec3f sMarioSetLocalPos = TVec3f(0.0f, -550.0f, 800.0f);
    static const char* sEventCameraName = "ダメージ";

    static const char* sJointName[7] = {"Tail1", "Tail2", "Tail3", "Tail4", "Tail5", "Tail6", "Tail7"};

    static f32 sKeepBendPower[7] = {30.0f, 25.0f, 20.0f, 15.0f, 10.0f, 10.0f, 10.0f};

    static f32 sJointRadius[7] = {80.0f, 70.0f, 60.0f, 55.0f, 45.0f, 50.0f};

    class DinoPackunParam : public AnimScaleParam {
    public:
        DinoPackunParam() {
            _0 = 0.1f;
            _10 = 30.0f;
            _14 = 0.60f;
            _18 = 0.059f;
            _1C = 3.0f;
            _20 = 0.07f;
            _24 = 0.12f;
            _28 = 0.92f;
        }
    };

    static DinoPackunParam sParam = DinoPackunParam();

};  // namespace

DinoPackun::DinoPackun(const char* pName)
    : LiveActor(pName), mTail(nullptr), mBall(nullptr), mFootPrint(nullptr), mShell(nullptr), mDemoPos(nullptr), mShellBreakModel(nullptr),
      mTailBall(nullptr), mCamTargetMtx(nullptr), mCameraInfo(nullptr), mSequence(nullptr), _B4(nullptr), _B8(nullptr), _BC(0.0f, 0.0f, 0.0f, 1.0f),
      _E8(0, 0, 1) {
    _F4 = -1;
    mCameraVec.x = 0.0f;
    mCameraVec.y = 0.0f;
    mCameraVec.z = 0.0f;
    _104 = 0.0f;
    _108 = 0.0f;
    _10C = -1;
    _110 = 0;
}

void DinoPackun::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    s32 vs = mSequence->getVsCount();
    const char* v5 = "DionPackun2";

    if (vs == 1) {
        v5 = "DionPackun";
    }

    initModelManagerWithAnm(v5, nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    MR::makeQuatAndFrontFromRotate(&_BC, &_E8, this);
    _CC.set< f32 >(_BC);
    _DC.set< f32 >(mPosition);
    initSound(8, false);
    MR::onCalcGravity(this);
    initHitSensor(3);
    MR::addHitSensorAtJointEnemy(this, "head", "Head", 8, 270.0f, sHeadHitOffset);
    MR::addHitSensorEnemy(this, "body", 8, 200.f, sBodyHitOffset);

    if (mSequence->isUseEggShell()) {
        MR::addHitSensorEnemy(this, "egg", 8, 400.0f, sEggHitOffset);
    }

    initBinder(150.0f, 150.0f, 0);
    initEffectKeeper(0, nullptr, false);
    initCamera(rIter);
    initTail();
    MR::initShadowFromCSV(this, "Shadow");
    initFootPrint();

    if (mSequence->isUseEggShell()) {
        initEggShell();
    }

    initBall();
    initScaleJointController();
    initDemoPosition(rIter);
    MR::declarePowerStar(this);
    MR::needStageSwitchReadA(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::startBck(this, "OpeningDemo", nullptr);
    MR::stopBck(this);
    MR::startBtp(this, "FlowerAll");
    MR::startBrk(this, "Normal");
    MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &DinoPackun::startSequence));
    MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);

    if (mSequence != nullptr) {
        mSequence->init();
    }

    makeActorAppeared();
    mTail->deactivate();
}

void DinoPackun::initTail() {
    mTail = new DinoPackunTail(9);
    DinoPackunTailRoot* root = new DinoPackunTailRoot("尻尾ルート", this);
    root->createJointController(this, "TailRoot");
    root->initWithoutIter();
    root->mKeepBendPower = 30.0f;
    mTail->addTailNode(root);

    for (u32 i = 0; i < 7; i++) {
        DinoPackunTailPart* p = new DinoPackunTailPart("尻尾ジョイント", this);
        MR::copyJointPos(this, sJointName[i], &p->mPosition);
        p->createJointController(this, sJointName[i]);
        f32 r = sJointRadius[i];
        p->mKeepBendPower = sKeepBendPower[i];
        p->_D4 = r;

        if (i >= 3) {
            p->_D9 = 0;
        }

        p->initWithoutIter();
        mTail->addTailNode(p);
    }

    mBall = new DinoPackunBall("尻尾先端球", this);
    MR::copyJointPos(this, "TailBall", &mBall->mPosition);
    mBall->mLinkLength = 120.0f;
    mBall->setWeakSensor(getSensor("head"));
    mBall->createJointController(this, "TailBall");
    mBall->initWithoutIter();
    mTail->addTailNode(mBall);
}

void DinoPackun::initFootPrint() {
    mFootPrint = new FootPrint("足跡", 32);
    mFootPrint->setTexture(MR::getTexFromArc("DinoPackunFootprint.bti", this));
    mFootPrint->_2C = 0.0f;
    mFootPrint->_30 = 100.0f;
    mFootPrint->_34 = 100.0f;
    mFootPrint->_38 = 10.0f;
}

void DinoPackun::initDemoPosition(const JMapInfoIter& rIter) {
    mDemoPos = new DinoPackunDemoPosition("デモ位置管理");
    mDemoPos->init(rIter);
}

void DinoPackun::initEggShell() {
    mShell = new DinoPackunEggShell("卵の殻", getSensor("head"), MR::getJointMtx(this, "EggShell"));
    mShell->initWithoutIter();

    if (mSequence->getVsCount() == 1) {
        MR::startBtp(mShell, "Vs1");
        MR::startBrk(mShell, "Vs1");
    } else {
        MR::startBtp(mShell, "Vs2");
        MR::startBrk(mShell, "Vs2");
    }

    mShellBreakModel = MR::createPartsModelEnemy(this, "卵の殻壊れモデル", "DinoPackunEggShellBreak", MR::getJointMtx(this, "EggShell"));
    MR::initLightCtrl(mShellBreakModel);
    mShellBreakModel->makeActorDead();
    if (mSequence->getVsCount() == 1) {
        MR::startBtp(mShellBreakModel, "Vs1");
    } else {
        MR::startBtp(mShellBreakModel, "Vs2");
    }
}

void DinoPackun::initBall() {
    mTailBall = MR::createPartsModelEnemy(this, "尻尾玉モデル", "DinoPackunTailBall", MR::getJointMtx(this, "TailBall"));
    MR::startBrk(mTailBall, "Normal");

    if (mSequence->getVsCount() == 1) {
        MR::startBtp(mTailBall, "Vs1");
    } else {
        MR::startBtp(mTailBall, "Vs2");
    }

    mTailBall->makeActorAppeared();
}

void DinoPackun::initCamera(const JMapInfoIter& rIter) {
    mCameraInfo = new ActorCameraInfo(rIter);
    MR::initAnimCamera(this, mCameraInfo, "OpeningDemo");
    MR::initAnimCamera(this, mCameraInfo, "CryDemo");
    MR::initAnimCamera(this, mCameraInfo, "AngryDemo");
    MR::initAnimCamera(this, mCameraInfo, "DownDemo");
    MR::initMultiActorCamera(this, rIter, &mCameraInfo, sEventCameraName);
    MR::getJMapInfoArg7WithInit(rIter, &_F4);

    if (_F4 != -1) {
        MR::declareCameraRegisterVec(this, _F4, &mCameraVec);
    }

    mCamTargetMtx = new CameraTargetMtx("カメラターゲットダミー");
}

void DinoPackun::initScaleJointController() {
    _B8 = new AnimScaleController(&sParam);
    _B4 = MR::createJointDelegatorWithNullChildFunc(this, &DinoPackun::hitScaleJoint, "Spine1");
}

void DinoPackun::makeActorDead() {
    LiveActor::makeActorDead();
    mTail->deactivate();
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void DinoPackun::startHitReaction() {
    MR::startSound(this, "SE_OJ_STAR_PIECE_HIT_STOP", -1, -1);
    if (_B8 != nullptr) {
        _B8->startHitReaction();
    }
}

bool DinoPackun::isHitReaction(s32 a1) const {
    if (_B8 != nullptr) {
        return _B8->isHitReaction(a1);
    }
    return false;
}

bool DinoPackun::hitScaleJoint(TPos3f* a1, const JointControllerInfo& a2) {
    TVec3f v5(_B8->_C);
    TVec3f v4;
    v4.x = v5.x;
    v4.y = v5.y;
    v4.z = v5.z;
    MR::preScaleMtx(*a1, v4);
    return true;
}

DinoPackunEggShell* DinoPackun::getEggShell() {
    return mShell;
}

PartsModel* DinoPackun::getEggBrokenModel() {
    return mShellBreakModel;
}

PartsModel* DinoPackun::getBallModel() {
    return mTailBall;
}

void DinoPackun::attackSensor(HitSensor* a1, HitSensor* a2) {
    if (mSequence != nullptr) {
        mSequence->attackSensor(a1, a2);
    }
}

bool DinoPackun::receiveMsgPlayerAttack(u32 msg, HitSensor* a1, HitSensor* a2) {
    if (mSequence != nullptr) {
        return mSequence->receiveMsgPlayerAttack(msg, a1, a2);
    }

    return false;
}

bool DinoPackun::receiveMsgPush(HitSensor* a1, HitSensor* a2) {
    if (mSequence != nullptr) {
        return mSequence->receiveMsgPush(a1, a2);
    }

    return false;
}

bool DinoPackun::receiveOtherMsg(u32 msg, HitSensor* a1, HitSensor* a2) {
    if (mSequence != nullptr) {
        return mSequence->receiveOtherMsg(msg, a1, a2);
    }

    return false;
}

void DinoPackun::attackSensorTail(HitSensor* a1, HitSensor* a2) {
    if (mSequence != nullptr) {
        mSequence->attackSensorTail(a1, a2);
    }
}

bool DinoPackun::receiveMsgPlayerAttackTail(u32 msg, HitSensor* a1, HitSensor* a2) {
    if (mSequence != nullptr) {
        return mSequence->receiveMsgPlayerAttackTail(msg, a1, a2);
    }

    return false;
}

void DinoPackun::startSequence() {
    if (mSequence != nullptr) {
        MR::invalidateClipping(this);
        mTail->activate();
        mSequence->start();
    }
}

void DinoPackun::control() {
    if (mSequence != nullptr) {
        mSequence->update();
    }

    if (_B8 != nullptr) {
        _B8->updateNerve();
    }

    updatePose();
    _110 = 1;
    calcAnim();
    _110 = 0;
    mTail->updateJoint();
    updateCameraInfo();
}

void DinoPackun::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _BC);
    MR::setBaseScale(this, mScale);

    if (_110) {
        mTail->registerPreCalcJointCallBack();
    } else {
        mTail->registerJointCallBack();
    }

    if (_B4 != nullptr) {
        _B4->registerCallBack();
    }
}

void DinoPackun::updatePose() {
    TVec3f* grav = &mGravity;
    f32 v3 = mGravity.dot(_E8);
    JMAVECScaleAdd(grav, &_E8, &_E8, -v3);
    if (MR::isNearZero(_E8)) {
        _BC.getZDir(_E8);
    } else {
        MR::normalize(&_E8);
    }

    MR::blendQuatUpFront(&_BC, -mGravity, _E8, 0.1f, 0.2f);
}

void DinoPackun::updateFootPrintNerve(s32 a2, s32 a3) {
    TVec3f v20;
    _BC.getXDir(v20);

    if (!(a2 % a3)) {
        TVec3f v16 = mGravity * -100.0f;
        TVec3f v17 = v20 * -100.0f;
        TVec3f v18(mPosition);
        v18.add(v17);
        TVec3f v19(v18);
        v19.add(v16);
        TVec3f v11 = mGravity * 200.0f;

        TVec3f v10;
        TVec3f v9;
        if (MR::getFirstPolyNormalOnLineToMap(&v10, v19, v11, &v9, nullptr)) {
            mFootPrint->addPrint(v9, _E8, v10, false);
        }
    } else {
        if (a3 / 2 == a2 % a3) {
            TVec3f v12 = mGravity * -100.0f;
            TVec3f v13 = v20 * 100.0f;
            TVec3f v14(mPosition);
            v14.add(v13);
            TVec3f v15(v14);
            v15.add(v12);
            TVec3f v8 = mGravity * 200.0f;

            TVec3f v7;
            TVec3f v6;
            if (MR::getFirstPolyNormalOnLineToMap(&v7, v15, v8, &v6, nullptr)) {
                mFootPrint->addPrint(v6, _E8, v7, true);
            }
        }
    }
}

void DinoPackun::updateCameraInfo() {
    if (_F4 != -1) {
        f32 v2 = 0.0;
        f32 v3 = (_108 + _104);
        _108 += _104;

        if (v3 < 0.0f) {
            v3 = v3;
        } else {
            v2 = 1.0f;
            if (v3 > 1.0f) {
                v2 = v2;
            } else {
                v2 = v3;
            }
        }

        _108 = v2;
        TVec3f v6(0, 0, 0);
        TVec3f v5;
        _BC.getYDir(v5);
        JMAVECScaleAdd(&v5, &mPosition, &v6, 300.0f);

        if (_108 > 0.0f) {
            TVec3f v4;
            mTail->getEndTailPosition(&v4);
            MR::vecBlend(v6, v4, &v6, (0.60f * _108));
        }

        mCameraVec.set< f32 >(v6);
        mCamTargetMtx->mMatrix.setTrans(v6);
    }
}

void DinoPackun::updateNormalVelocity() {
    if (!MR::isBindedGround(this)) {
        MR::addVelocityToGravity(this, 1.0f);
    }

    if (MR::isBindedWall(this)) {
        MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    }

    MR::attenuateVelocity(this, 0.89f);
}

void DinoPackun::updateRunVelocity() {
    updateNormalVelocity();
}

void DinoPackun::appearStarPiece(s32 num) {
    TVec3f pos;
    MR::copyJointPos(this, "Head", &pos);
    MR::appearStarPieceToDirection(this, pos, -mGravity, num, 50.0f, 60.0f, false);
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
}

bool DinoPackun::isSensorEgg(const HitSensor* pSensor) const {
    return getSensor("egg") == pSensor;
}

void DinoPackun::resetPosition() {
    _BC.set< f32 >(_CC);
    _BC.getZDir(_E8);
    MR::resetPosition(this, _DC);
    MR::zeroVelocity(this);

    TPos3f v28;
    v28.setRotateQuaternionInline(_BC);
    v28.setTrans(mPosition);
    TVec3f v27;
    v28.mult(sMarioSetLocalPos, v27);
    TVec3f v26;
    MR::calcGravityVector(this, v27, &v26, nullptr, 0);

    TVec3f v24(mPosition);
    JMathInlineVEC::PSVECSubtract(&v27, &v24, &v24);
    TVec3f v25;
    v25 = v24;
    MR::makeMtxUpFrontPos(&v28, -v26, v25, v27);
    MR::setPlayerBaseMtx(v28);
}

void DinoPackun::adjustTailRootPosition(const TVec3f& a1, f32 a2) {
    TVec3f v20;
    _BC.rotateVec(v20, a1);
    v20.add(mPosition);
    TVec3f v19;
    v19 = mTail->getNode(1)->mPosition;
    DinoPackunTailNode* node = mTail->getNode(1);
    TVec3f v16 = v19 * (1.0f - a2);
    TVec3f v17 = v20 * a2;
    TVec3f v18(v17);
    v18.add(v16);
    node->mPosition.set< f32 >(v18);
}

void DinoPackun::activateParts() {
    mTail->activate();
}

void DinoPackun::onMovementParts() {
    mTail->onMovement();
    MR::requestMovementOn(mDemoPos);
    MR::requestMovementOn(mFootPrint);
    if (mShell != nullptr) {
        MR::requestMovementOn(mShell);
    }

    if (mShellBreakModel != nullptr) {
        MR::requestMovementOn(mShellBreakModel);
    }

    if (mTailBall != nullptr) {
        MR::requestMovementOn(mTailBall);
    }
}

void DinoPackun::onAimTailBall(s32 a1) {
    if (a1 <= 0) {
        _104 = 0.0f;
        _108 = 1.0f;
    } else {
        _104 = 1.0f / a1;
    }
}

void DinoPackun::offAimTailBall(s32 a1) {
    if (a1 <= 0) {
        _104 = 0.0f;
        _108 = 0.0f;
    } else {
        _104 = -1.0f / a1;
    }
}

void DinoPackun::startDemo() {
    onMovementParts();
    MR::turnOffImageEffect();
}

void DinoPackun::startDemoAndReset() {
    mTail->deactivate();
    resetPosition();
    mFootPrint->clear();
    onMovementParts();
    MR::turnOffImageEffect();
}

void DinoPackun::endDemo(const char* pName) {
    MR::endDemo(this, pName);
    MR::setImageEffectControlAuto();
}

void DinoPackun::startDamageCamera() {
    CameraTargetArg arg(nullptr, mCamTargetMtx, nullptr, nullptr);
    MR::startMultiActorCameraTargetOther(this, mCameraInfo, sEventCameraName, arg, -1);
    _10C = 0;
}

void DinoPackun::endDamageCamera() {
    if (_10C != -1) {
        MR::endMultiActorCamera(this, mCameraInfo, sEventCameraName, false, -1);
        _10C = -1;
    }
}

DinoPackun::~DinoPackun() {
    return;
}

namespace MR {
    NameObj* createDinoPackunVs1(const char* pName) {
        DinoPackun* p = new DinoPackun(pName);
        p->mSequence = new DinoPackunVs1(p);
        return p;
    }

    NameObj* createDinoPackunVs2(const char* pName) {
        DinoPackun* p = new DinoPackun(pName);
        p->mSequence = new DinoPackunVs2(p);
        return p;
    }
};  // namespace MR
