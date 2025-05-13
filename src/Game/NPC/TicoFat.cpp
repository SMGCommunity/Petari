#include "Game/NPC/TicoFat.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/Effect/MultiEmitter.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/SpinDriverShootPath.hpp"
#include "Game/MapObj/StarPieceDirector.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Screen/FullnessMeter.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include <cstdio>

namespace NrvTicoFat {
    NEW_NERVE(TicoFatNrvReaction, TicoFat, Reaction);
    NEW_NERVE(TicoFatNrvPrep, TicoFat, Prep);
    NEW_NERVE(TicoFatNrvWait, TicoFat, Wait);
    NEW_NERVE(TicoFatNrvPoint, TicoFat, Point);
    NEW_NERVE(TicoFatNrvEat, TicoFat, Eat);
    NEW_NERVE(TicoFatNrvChem, TicoFat, Chem);
    NEW_NERVE(TicoFatNrvTest, TicoFat, Test);
    NEW_NERVE(TicoFatNrvFullness, TicoFat, Fullness);
    NEW_NERVE(TicoFatNrvMeta, TicoFat, Meta);
    NEW_NERVE(TicoFatNrvDemo, TicoFat, Demo);
    NEW_NERVE(TicoFatNrvFly, TicoFat, Fly);
    NEW_NERVE(TicoFatNrvWipeOut, TicoFat, WipeOut);
    NEW_NERVE(TicoFatNrvWipeIn, TicoFat, WipeIn);
    NEW_NERVE(TicoFatNrvInfo, TicoFat, Info);
    NEW_NERVE(TicoFatNrvAfter, TicoFat, After);
};

namespace {
    class TicoFatParam : public AnimScaleParam {
    public:
        TicoFatParam() {
            _20 = 0.12f;
            _24 = 0.12f;
            _28 = 0.89999998f;
        }
    };

    static const char* sInfoMessageID = "InformationPlanet";
    static TicoFatParam sParam;
    static Color8 hPointLight = Color8(0xFF, 0x7D, 0xE6, 0xFF);
};

const char* TicoFat::getActionName(const char *pName) {
    if (calcScale() > 1.6f) {
        if (!strcmp(pName, "Eat")) {
            return "eat2";
        }
        if (!strcmp(pName, "EatStart")) {
            return "eatstart2";
        }
        if (!strcmp(pName, "Wait")) {
            return "wait2";
        }
        if (!strcmp(pName, "Excite")) {
            return "excite2";
        }
        if (!strcmp(pName, "Joy")) {
            return "joy2";
        }
    }
    else {
        if (calcScale() > 1.3f) {
            if (!strcmp(pName, "Eat")) {
                return "eat1";
            }
            if (!strcmp(pName, "EatStart")) {
                return "eatstart1";
            }
            if (!strcmp(pName, "Wait")) {
                return "wait1";
            }
            if (!strcmp(pName, "Excite")) {
                return "excite1";
            }
            if (!strcmp(pName, "Joy")) {
                return "joy1";
            }
        }
        else {
            if (!strcmp(pName, "Eat")) {
                return "eat0";
            }
            if (!strcmp(pName, "EatStart")) {
                return "eatstart0";
            }
            if (!strcmp(pName, "Wait")) {
                return "wait0";
            }
            if (!strcmp(pName, "Excite")) {
                return "excite0";
            }
            if (!strcmp(pName, "Joy")) {
                return "joy0";
            }
        }
    }

    return pName;
}

TicoFat::TicoFat(const char *pName) : NPCActor(pName) {
    mCameraInfo = nullptr;
    mItem = nullptr;
    mShootPath = nullptr;
    _170 = 0;
    _1DC = -1;
    _1EC = -1;
    _1F0 = 5;
    _1FC = 100.0f;
    _200 = 0;
    _204 = 0;
    _17C.identity();
    _1AC.identity();
}

void TicoFat::makeArchiveList(NameObjArchiveListCollector *pCollector, const JMapInfoIter &rIter) {
    NPCActorItem item("TicoFat");
    if (MR::getNPCItemData(&item, 0)) {
        NPCActor::addArchive(pCollector, item);
    }
}

void TicoFat::init(const JMapInfoIter &rIter) {
    NPCActorCaps caps("TicoFat");
    NPCActorItem item("TicoFat");
    caps.setDefault();
    caps.mWaitNerve = &NrvTicoFat::TicoFatNrvPrep::sInstance;
    caps._E = 0;
    caps.mSensorSize = 100.0f;
    caps.mSensorOffset.y = 30.0f;
    caps._44 = "Center";
    caps.mUseShadow = true;
    caps.mUseRail = false;
    caps._6C = "Center";
    caps._80 = 110.0f;
    caps._38 = 0;
    caps._30 = 6;
    caps._58 = 2;
    _178 = 0;
    _174 = 0;
    mStartEat = false;
    _1F4 = -1;
    mScaleController = new AnimScaleController(&sParam);
    NPCActor::initialize(rIter, caps);
    s32 itemType = _1DC;
    MR::getJMapInfoArg0NoInit(rIter, &itemType);
    _1DC = itemType;

    switch (_1DC) {
        case 0:
            mItem = MR::createKinokoOneUp();
            break;
        case 1:
            mItem = MR::createKinokoSuper();
            break;
        case -1:
        case 2:
            break;
    }

    if (mItem != nullptr) {
        mItem->mPosition.set(mPosition);
        MR::initDefaultPos(mItem, rIter);
    }

    s32 numItem = 10;
    MR::getJMapInfoArg1NoInit(rIter, &numItem);
    _1E0 = _1E4 = numItem;
    mCurrentFed = 0;
    MR::addHitSensorAtJointEye(this, "Mouth", "Mouth", 8, 30.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initStarPieceSaveData(rIter);
    MR::declareStarPieceReceiver(this, _1E0);
    mMsgCtrl = MR::createTalkCtrlDirect(this, rIter, "Common_TicoFat001", TVec3f(0.0f, 220.0f, 0.0f), nullptr);
    _16C = MR::createTalkCtrlDirect(this, rIter, "Common_TicoFat000", TVec3f(0.0f, 220.0f, 0.0f), nullptr);
    MR::registerEventFunc(mMsgCtrl, TalkMessageFunc_Inline(this, &TicoFat::eventFunc));
    MR::setMessageArg(mMsgCtrl, _1E0);
    MR::setMessageArg(_16C, _1E0);
    MR::setDistanceToTalk(mMsgCtrl, 280.0f);
    MR::setDistanceToTalk(_16C, 280.0f);
    MR::onRootNodeAutomatic(mMsgCtrl);
    MR::onStartOnlyFront(mMsgCtrl);
    MR::onStartOnlyFront(_16C);
    s32 arg2 = -1;
    MR::getJMapInfoArg2NoInit(rIter, &arg2);
    if (arg2 != -1) {
        startEat();
    }

    s32 arg3 = -1;
    MR::getJMapInfoArg3NoInit(rIter, &arg3);
    _1F4 = arg3;
    mMeter = new FullnessMeter(this, _1E4, _1E4 - _1E0);
    mMeter->initWithoutIter();
    mMeter->setNumber(_1E4 - _1E0);
    MR::initMultiActorCamera(this, rIter, &mCameraInfo, "変身");
    MR::initMultiActorCamera(this, rIter, &mCameraInfo, "飛行");
    MR::initMultiActorCamera(this, rIter, &mCameraInfo, "喜び");
    MR::declareEventCameraProgrammable("デブチコカメラ");
    MR::useStageSwitchWriteA(this, rIter);
    MR::getNPCItemData(&item, 0);
    equipment(item, 0);
    MR::startAction(_94, "TicoFatGoodsStarPiece");
    MR::startAction(_98, "TicoFatGoodsStarPiece");
    MR::setBpkFrame(_94, 0.0f);
    MR::setBpkFrame(_98, 100.0f);
    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        if (_1DC == -1) {
            mShootPath = new SpinDriverShootPath();
            mShootPath->initUsingParabolic(rIter, mPosition);
            MR::getJMapInfoArg4NoInit(rIter, &_1FC);
        }
    }

    MR::startBva(this, "Small0");
    MR::startBrk(this, "Normal");
    MR::setBrkFrameAndStop(this, 0.0f);
    setScale(calcScale());
    MR::joinToGroupArray(this, rIter, "惑星出現", 32);
    AstroDemoFunction::tryRegisterSimpleCastIfAstroGalaxy(this);
    setDefaults();
    _12C = 1000.0f;
    const char* waitActionName = getActionName("Wait");
    _FC = waitActionName;
    _100 = waitActionName;
    const char* talkActionName = getActionName("Talk");
    _104 = talkActionName;
    _108 = talkActionName;
    
    if (!enableAppear()) {
        makeActorDead();
    }
}

void TicoFat::initAfterPlacement() {
    MR::extractMtxYDir(getBaseMtx(), &mGravity);
    mGravity.invert();
    if (!_1E0) {
        disappear(false);
        makeActorDead();
    }
}

void TicoFat::kill() {
    MR::validateClipping(this);
    mMeter->requestDisappear();
    NPCActor::kill();
}

// stack isn't quite there
void TicoFat::setCameraParam() {
    TVec3f trans, yDir, zDir, xDir, v18;
    MR::extractMtxXYZDir(getBaseMtx(), &xDir, &yDir, &zDir);
    MR::extractMtxTrans(getBaseMtx(), &trans);
    xDir.set<f32>(((1.0f - (2.0f * (_B0.y * _B0.y))) - (2.0f * (_B0.z * _B0.z))), ((2.0f * (_B0.x * _B0.y)) + (2.0f * (_B0.h * _B0.z))), ((2.0f * (_B0.x * _B0.z)) - (2.0f * (_B0.h * _B0.y))));
    yDir.set<f32>(((2.0f * (_B0.x * _B0.y)) - (2.0f * (_B0.h * _B0.z))), ((1.0f - (2.0f * (_B0.x * _B0.x))) - (2.0f * (_B0.z * _B0.z))), ((2.0f * (_B0.y * _B0.z)) + (2.0f * (_B0.h * _B0.x))));
    zDir.set<f32>(((2.0f * (_B0.x * _B0.z)) + (2.0f * (_B0.h * _B0.y))), ((2.0f * (_B0.y * _B0.z)) - (2.0f * (_B0.h * _B0.x))), ((1.0f - (2.0f * (_B0.x * _B0.x))) - (2.0f * (_B0.y * _B0.y))));
    
    v18.setPS2(_C0);
    TVec3f* ptr = &v18;
    MR::setProgrammableCameraParam("デブチコカメラ", (*ptr + (xDir * 0.0f) + (yDir * 0.0f)) + (zDir * 0.0f), (*ptr + (xDir * 0.0f)) + ((yDir * 100.0f)) + (zDir * MR::getLinerValueFromMinMax(calcScale(), 1.0f, 1.9f, 1300.0f, 1680.0f)), yDir, false);
}

void TicoFat::control() {
    NPCActor::control();
    TVec3f trans;
    MR::extractMtxTrans(MR::getJointMtx(this, "Center"), &trans);
    MR::requestPointLight(this, TVec3f(trans), hPointLight, 0.99864602f, -1);
    if (isNerve(&NrvTicoFat::TicoFatNrvPrep::sInstance) || isNerve(&NrvTicoFat::TicoFatNrvWait::sInstance) || isNerve(&NrvTicoFat::TicoFatNrvPoint::sInstance)
        || isNerve(&NrvTicoFat::TicoFatNrvEat::sInstance) || isNerve(&NrvTicoFat::TicoFatNrvChem::sInstance)
        || isNerve(&NrvTicoFat::TicoFatNrvFullness::sInstance) || isNerve(&NrvTicoFat::TicoFatNrvDemo::sInstance)) {
            MR::startLevelSound(this, "SE_SM_LV_TICO_WAIT", -1, -1, -1);
        }

    updateGoods();
    _204--;
    if (mStartEat) {
        if (!_1E0) {
            MR::invalidateClipping(this);
            if (isNerve(&NrvTicoFat::TicoFatNrvPoint::sInstance) || isNerve(&NrvTicoFat::TicoFatNrvEat::sInstance)
                || isNerve(&NrvTicoFat::TicoFatNrvChem::sInstance) || isNerve(&NrvTicoFat::TicoFatNrvWait::sInstance)) {
            
                setNerve(&NrvTicoFat::TicoFatNrvTest::sInstance);
            }
        }

        if (!_1E0 || MR::isPlayerElementModeInvincible() || MR::isPlayerFlying()) {
            _178 = 0;
            _174 = 0;
        }
        else {
            updateGuidance();
            if (_174 > 0) {
                updatePointing();
            }
            else {
                _178 = 0;
            }
        }

        if (_174 > 0) {
            if (_170 == 0) {
                _170 = 1;
                CameraTargetArg arg(this);
                MR::startGlobalEventCamera("デブチコカメラ", arg, 120);
                setCameraParam();
            }
        }
        else {
            if (_170) {
                _170 = 0;
                MR::endGlobalEventCamera("デブチコカメ", 120, true);
            }
        }

        if (_178 > 0) {
            if (MR::isIntervalStep(this, _1F0)) {
                if (MR::testCorePadButtonB(0)) {
                    if (!MR::isDemoActive() && !isNerve(&NrvTicoFat::TicoFatNrvReaction::sInstance)) {
                        if (_1E0 && MR::getStarPieceNum() > 0) {
                            shootStarPiece();
                        }
                    }
                }
            }

            mMeter->requestAppear();
        }
        else {
            mMeter->requestDisappear();
        }
    }
}

bool TicoFat::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return false;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        MR::limitedStarPieceHitSound();
        _E5 = 1;
        return true;
    }
    else if (MR::isMsgStarPieceGift(msg)) {
        receiveStarPiece(MR::getNumStarPieceGift(msg));
        return true;
    }
    else {
        return NPCActor::receiveMsgPlayerAttack(msg, a2, a3);
    }
}

void TicoFat::calcAndSetBaseMtx() {
    NPCActor::calcAndSetBaseMtx();
}

bool TicoFat::eventFunc(u32 var) {
    switch (var) {
        case 0:
            startEat();
            break;
        case 1:
            return false;
    }

    return true;
}

f32 TicoFat::calcScale() {
    return (1.0f + ((f32)(_1E4 - _1E0) / _1E4) * 0.89999998f);
}

void TicoFat::setScale(f32 scale) {
    f32 x = mScale.x;
    mScale.y = scale;
    mScale.x = scale;
    mScale.z = scale;
    if ((x <= 1.3f && 1.3f < scale) || (x <= 1.6f && 1.6f < scale)) {
        mScaleController->startHitReaction();
    }

    f32 v5 = (scale - 1.0f) / 0.89999998f;
    if (1.0f >= v5) {
        v5 = v5;
    }
    else {
        v5 = 1.0f;
    }

    f32 frameMax = v5 * MR::getBrkFrameMax(this);
    MR::startBrk(this, "FatRed");
    MR::setBrkFrameAndStop(this, frameMax);
    MR::setSensorRadius(this, "Body", (100.0f * scale));
    MR::setStarPointerTargetRadius3d(this, 110.0f * scale);
    f32 v7 = 1.5f;
    if (scale >= 1.5f) {
        v7 = v7;
    }
    else {
        v7 = scale;
    }

    MR::setDistanceToTalk(mMsgCtrl, (280.0f * v7));
    f32 v8 = 1.5f;
    if (scale >= 1.5f) {
        v8 = v8;
    }
    else {
        v8 = scale;
    }
    MR::setDistanceToTalk(_16C, (280.0f * v8));
    MR::setMessageBalloonFollowOffset(mMsgCtrl, TVec3f(0.0f, 220.0f * scale, 0.0f));
    MR::setMessageBalloonFollowOffset(_16C, TVec3f(0.0f, 220.0f * scale, 0.0f));
}

void TicoFat::setMessage(s32 msg) {
    s32 nodeCount = 0;
    f32 v5 = ((f32)_1E0 / (f32)_1E4);
    MR::resetNode(_16C);

    switch (msg) {
        case 1:
            {
                if (1.0f == v5) {
                    nodeCount = 1;
                }
                else if (v5 >= 0.5f) {
                    nodeCount = 2;
                }
                else if (v5 >= 0.25f) {
                    nodeCount = 3;
                }
                else if (v5 >= 0.050000001f) {
                    nodeCount = 4;
                }
                else {
                    nodeCount = 5;
                }
            }
            break;
        case 2:
            nodeCount = 6;
            break;
        case 0:
            break;
    }

    for (s32 i = 0; i < nodeCount; i++) {
        MR::forwardNode(_16C);
    }
}

void TicoFat::shootStarPiece() {
    if (MR::giftStarPieceToTarget(getSensor("Mouth"), 1)) {
        mCurrentFed++;
        MR::tryRumblePadVeryWeak(this, 0);
    }
}

void TicoFat::receiveStarPiece(s32 num) {
    s32 val = _1E0;
    if (val >= num) {
        val = num;
    }

    mCurrentFed -= val;
    _1E0 -= val;
    addStarPieceSaveData(val);
    setScale(calcScale());
    mMeter->setNumber(_1E4 - _1E0);
    MR::tryRumblePadVeryWeak(this, 0);
    startAbsorbSound();
    MR::setMessageArg(mMsgCtrl, _1E0);
    MR::setMessageArg(_16C, _1E0);
    if (_174 > 0) {
        setCameraParam();
    }

    if (isNerve(&NrvTicoFat::TicoFatNrvEat::sInstance)) {
        MR::setBckRate(this, MR::calcNerveValue(this, 0, 180, 1.0f, 1.6f));
    }
    else {
        if (!isEmptyNerve()) {
            popNerve();
        }

        setNerve(&NrvTicoFat::TicoFatNrvEat::sInstance);
    }
}

void TicoFat::startEat() {
    mStartEat = true;
}

bool TicoFat::enableAppear() {
    return true;
}

void TicoFat::initStarPieceSaveData(const JMapInfoIter &rIter) {
    if (MR::getJMapInfoArg7WithInit(rIter, &_1EC)) {
        s32 starPieceNum = MR::getStarPieceNumGivingToTicoSeed(_1EC);
        _1E0 = (_1E4 - starPieceNum <= 0) ? 0 : _1E4 - starPieceNum;
    }
}

void TicoFat::addStarPieceSaveData(s32) {
    
}

void TicoFat::appearInformation() const {
    MR::appearInformationMessage(MR::getGameMessageDirect(sInfoMessageID), true);
}

void TicoFat::disappear(bool a1) {
    if (mItem != nullptr) {
        TPos3f pos;
        pos.setInline(getBaseMtx());
        TVec3f trans;
        MR::extractMtxTrans(MR::getJointMtx(this, "Spine1"), &trans);
        pos.mMtx[0][3] = trans.x;
        pos.mMtx[1][3] = trans.y;
        pos.mMtx[2][3] = trans.z;
        MR::appearKinokoOneUpPop(mItem, pos, 15.0f);
    }

    if (a1) {
        if (_1EC != -1) {
            MR::addStarPieceGivingToTicoSeed(_1EC, _1E4);
        }
        MR::callAppearAllGroupMember(this);
    }
    else {
        MR::callMakeActorAppearedAllGroupMember(this);
    }

    if (MR::isValidSwitchA(this)) {
        MR::onSwitchA(this);
    }
}

bool TicoFat::tryMetamorphosis() {
    if (_1DC == -1) {
        setNerve(&NrvTicoFat::TicoFatNrvDemo::sInstance);
        MR::callRequestMovementOnAllGroupMember(this);
        MR::requestMovementOn(_94);
        MR::requestMovementOn(_98);
    }
    else {
        setNerve(&NrvTicoFat::TicoFatNrvMeta::sInstance);
    }

    return true;
}

void TicoFat::emitScreenEffect() {
    MR::emitEffect(this, "TicoFatScreenEffect")->setHostMtx(_17C);
    MR::emitEffect(this, "TicoFatScreenEffectLight")->setHostMtx(_1AC);
}

void TicoFat::updateScreenEffect() {
    const TVec3f camPos = MR::getCamPos();
    TVec3f v19(mPosition);
    JMathInlineVEC::PSVECSubtract(&v19, &camPos, &v19);
    MR::normalizeOrZero(&v19);
    MR::makeMtxFrontUpPos(&_17C, -MR::getCamZdir(), MR::getCamYdir(), MR::getCamPos() + (v19 * 500.0f));
    MR::makeMtxFrontUpPos(&_1AC, -MR::getCamZdir(), MR::getCamYdir(), mPosition);
}

s32 TicoFat::getDanceSeTranspose() const {
    
}
