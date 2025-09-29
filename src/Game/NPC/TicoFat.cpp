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
#include "Game/Util/NPCUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
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
    mParam._14 = waitActionName;
    mParam._18 = waitActionName;
    const char* talkActionName = getActionName("Talk");
    mParam._1C = talkActionName;
    mParam._20 = talkActionName;
    
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

bool TicoFat::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
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
        return NPCActor::receiveMsgPlayerAttack(msg, pSender, pReceiver);
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
    s32 val = (_1E4 - _1E0 - 1) / 50;
    if (val < 0) {
        val = 0;
    }

    if (val > 32) {
        return 32;
    }

    return val;
}

void TicoFat::startAbsorbSound() {
    if (_204 <= 0) {
        MR::startSound(this, "SE_SM_TICOFAT_ABSORB", -1, -1);
        _204 = 5;
    }
}

static s32 data_806B2194 = 0;
static s32 data_806B2198 = 0;

void TicoFat::updateGuidance() {
    bool isNear = MR::isNearPlayer(_16C, -1.0f);
    if (isNear && MR::requestTicoSeedGuidanceForce()) {
        MR::requestCounterLayoutAppearanceForTicoEat(false);
    }

    TVec3f screenPos;
    if (MR::calcScreenPosition(&screenPos, mPosition)) {
        bool req = MR::requestTicoSeedGuidance(_1F4);
        isNear = (isNear | req) != 0;
        if (req) {
            MR::requestCounterLayoutAppearanceForTicoEat(0);
        }
    }

    if (isNear) {
        _174 = 8;
    }
    else {
        s32 a = _174 - 1;
        _174 = *(data_806B2194 < a ? &a : &data_806B2194);
    }
}

void TicoFat::updatePointing() {
    bool isPointing;

    if (isNerve(&NrvTicoFat::TicoFatNrvEat::sInstance)) {
        isPointing = MR::isStarPointerPointing1PWithoutCheckZ(this, nullptr, false, false);
    }
    else {
        isPointing = MR::isStarPointerPointing(this, 0, false, nullptr);
    }

    if (isPointing) {
        _178 = 4;
    }
    else {
        s32 a = _178 - 1;
        _178 = *(data_806B2198 < a ? &a : &data_806B2198);
    }
}

void TicoFat::updateGoods() {
    if (MR::isNearPlayer(this, 2000.0f)) {
        MR::tryStartBck(_94, "TicoFatGoodsStarPiece", nullptr);
        MR::tryStartBck(_98, "TicoFatGoodsStarPiece", nullptr);
    }
    else {
        MR::tryStartBck(_94, "TicoFatGoodsStarPieceFar", nullptr);
        MR::tryStartBck(_98, "TicoFatGoodsStarPieceFar", nullptr);
    }
}

void TicoFat::exeReaction() {
    if (MR::isFirstStep(this) && !_D9) {
        mScaleController->startHitReaction();
    }

    if (_D8) {
        MR::startSound(this, "SE_SM_NPC_TRAMPLED", -1, -1);
        MR::startSound(this, "SE_SV_TICOFAT_TRAMPLED", -1, -1);
    }

    if (isPointingSe()) {
        MR::startDPDHitSound();
        MR::startSound(this, "SE_SV_TICOFAT_POINT", -1, -1);
    }

    if (_D9) {
        MR::startSound(this, "SE_SM_TICOFAT_SPIN", -1, -1);
    }

    if (_DB) {
        MR::startSound(this, "SE_SV_TICOFAT_STAR_PIECE_HIT", -1, -1);
    }

    if (MR::tryStartReactionAndPopNerve(this)) {
        return;
    }
}

void TicoFat::exePrep() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, getActionName("Wait"));
    }

    if (!MR::tryStartReactionAndPushNerve(this, &NrvTicoFat::TicoFatNrvReaction::sInstance) && !MR::isPlayerElementModeInvincible()) {
        if (mStartEat) {
            setNerve(&NrvTicoFat::TicoFatNrvWait::sInstance);
        }
        else if (mMsgCtrl != nullptr) {
            MR::tryTalkNearPlayerAndStartTalkAction(this);
        }
    }
}

void TicoFat::exeWait() {
    if (MR::isFirstStep(this)) {
        const char* exciteAction = getActionName("Excite");
        mParam._14 = getActionName("Wait");
        mParam._1C = exciteAction;
        mParam._18 = 0;
        mParam._20 = 0;
        mParam._0 = 0;
        mParam._1 = 0;
        setMessage(1);
    }

    if (!MR::tryStartReactionAndPushNerve(this, &NrvTicoFat::TicoFatNrvReaction::sInstance)) {
        if (_178 > 0) {
            setNerve(&NrvTicoFat::TicoFatNrvPoint::sInstance);
        }
        else {
            turnToDefault(mParam._8);
            MR::tryTalkNearPlayer(_16C);
            MR::tryStartTalkAction(this);
        }
    }
}

void TicoFat::exePoint() {
    if (MR::isFirstStep(this)) {
        setMessage(0);
        MR::startAction(this, getActionName("Excite"));
    }

    if (!MR::tryStartReactionAndPushNerve(this, &NrvTicoFat::TicoFatNrvReaction::sInstance)) {
        if (_178 <= 0) {
            setNerve(&NrvTicoFat::TicoFatNrvWait::sInstance);
        }
        else {
            turnToDefault(mParam._8);
            MR::tryTalkForce(_16C);
        }
    }
}

void TicoFat::exeEat() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, getActionName("EatStart"));
    }
    else {
        f32 frame = MR::getBckFrame(this);
        if (MR::tryStartAction(this, getActionName("Eat"))) {
            MR::setBckFrame(this, frame);
        }
    }

    MR::startLevelSound(this, "SE_SM_LV_TICOFAT_EATING", getDanceSeTranspose(), -1, -1);
    if (!mCurrentFed) {
        setNerve(&NrvTicoFat::TicoFatNrvChem::sInstance);
    }
}

void TicoFat::exeChem() {
    if (MR::isFirstStep(this)) {
        _200 = 5;
        MR::startAction(this, getActionName("Chem"));
    }

    if (_200 > 0) {
        _200--;
        MR::startLevelSound(this, "SE_SM_LV_TICOFAT_EATING", getDanceSeTranspose(), -1, -1);
    }

    if (MR::isBckLooped(this)) {
        if (_178 > 0) {
            _174 = 8;
            setNerve(&NrvTicoFat::TicoFatNrvPoint::sInstance);
        }
        else {
            _174 = 0;
            setNerve(&NrvTicoFat::TicoFatNrvWait::sInstance);
        }
    }
}

void TicoFat::exeTest() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAction(this, getActionName("Excite"));
        setMessage(2);
    }

    if (MR::tryStartDemo(this, "変身")) {
        MR::invalidateHitSensors(this);
        MR::stopPlayerFpView();
        MR::tryTalkForceWithoutDemo(_16C);

        if (_1EC == 6) {
            MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "喜び", -1);
        }

        MR::requestMovementOn(_94);
        MR::requestMovementOn(_98);
        mMeter->pauseOff();
        setNerve(&NrvTicoFat::TicoFatNrvFullness::sInstance);
    }
}

void TicoFat::exeFullness() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAction(this, getActionName("Joy"));
        MR::startSound(this, "SE_SM_TICOFAT_MAX", -1, -1);
    }

    MR::startLevelSound(this, "SE_SM_LV_TICOFAT_GLAD", getDanceSeTranspose(), -1, -1);
    if (MR::tryTalkForceWithoutDemoAtEnd(_16C)) {
        tryMetamorphosis();
    }
}

void TicoFat::exeMeta() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, getActionName("Metamorphosis"));
        MR::startSound(this, "SE_SM_TICOFAT_META", -1, -1);
        MR::startSound(this, "SE_SM_TICOFAT_META_ITEM", -1, -1);
    }

    if (MR::isBckStopped(this)) {
        MR::startSound(this, "SE_SM_METAMORPHOSE_SMOKE", -1, -1);
        disappear(true);
        kill();
    }
}

void TicoFat::exeDemo() {
    if (MR::isFirstStep(this)) {
        MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "変身", -1);
        MR::startAction(this, getActionName("Demo"));
        MR::startSound(this, "SE_SM_TICOFAT_META", -1, -1);
    }

    MR::turnPlayerToActor(this, 10.0f);
    if (MR::isStep(this, 89)) {
        MR::startBrk(this, "FatRed");
        MR::setBrkFrameAndStop(this, 0.0f);
    }

    if (MR::isStep(this, 90)) {
        MR::startSound(this, "SE_SM_METAMORPHOSE_SMOKE", -1, -1);
        MR::hideModelAndOnCalcAnim(this);
        if (_94) {
            MR::hideModelAndOnCalcAnim(_94);
        }

        if (_98) {
            MR::hideModelAndOnCalcAnim(_98);
        }
    }

    if (MR::isBckStopped(this)) {
        MR::endMultiActorCamera(this, mCameraInfo, "変身", false, -1);
        if (_1DC == -1 && MR::isExistRail(this)) {
            setNerve(&NrvTicoFat::TicoFatNrvFly::sInstance);
        }
        else {
            MR::setNPCActorPos(this, mPosition);
            setNerve(&NrvTicoFat::TicoFatNrvWipeOut::sInstance);
        }
    }
}

void TicoFat::exeFly() {
    if (MR::isFirstStep(this)) {
        _1F8 = 0.0f;
        MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "飛行", -1);
        MR::startAction(this, getActionName("Fly"));
        MR::startSound(this, "SE_DM_TICOFAT_MORPH_FLY", -1, -1);
        MR::tryRumblePadWeak(this, 0);
    }

    MR::tryRumblePadVeryWeak(this, 0);
    f32 v4 = _1F8 / mShootPath->getTotalLength();
    if (1.0f >= v4) {
        v4 = v4;
    }
    else {
        v4 = 1.0f;
    }
    
    f32 easeIn;

    if (v4 < 0.5f) {
        easeIn = MR::getEaseInValue((2.0f * v4) * (2.0f * v4), 0.0f, 0.5f, 1.0f);
    }
    else {
        f32 v6 = (2.0f * (1.0f - v4));
        easeIn = (1.0f - MR::getEaseInValue((v6 * v6), 0.0f, 0.5f, 1.0f));
    }

    TVec3f v8, shootPos, direction;
    mShootPath->calcPosition(&shootPos, easeIn);
    mShootPath->calcDirection(&direction, easeIn, 0.0099999998f);
    TVec3f up;
    MR::calcUpVec(&up, this);
    MR::blendQuatFrontUp(&_A0, _A0, v8, up, 0.1f, 0.0f);
    MR::setNPCActorPos(this, shootPos);
    _1F8 += _1FC;
    if (easeIn >= 1.0f) {
        setNerve(&NrvTicoFat::TicoFatNrvWipeOut::sInstance);
    }
}

void TicoFat::exeWipeOut() {
    if (MR::isFirstStep(this)) {
        MR::stopBck(this);
        MR::startSystemSE("SE_DM_TICOFAT_MORPH_WIPE_OUT", -1, -1);
        emitScreenEffect();
        MR::closeWipeWhiteFade(60);
        MR::tryRumblePadMiddle(this, 0);
        MR::shakeCameraNormal();
    }

    updateScreenEffect();

    if (!MR::isWipeActive()) {
        MR::emitEffect(this, "TicoFatScreenEFfectFog")->setHostMtx(_17C);
        MR::deleteEffect(this, "TicoFatScreenEffect");
        setNerve(&NrvTicoFat::TicoFatNrvWipeIn::sInstance);
    }
}

void TicoFat::exeWipeIn() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_DM_TICOFAT_MORPH_WIPE_IN", -1, -1);
    }

    if (!MR::isLessStep(this, 60)) {
        if (MR::isStep(this, 60)) {
            if (MR::isValidSwitchA(this)) {
                MR::onSwitchA(this);
            }

            MR::startBck(this, getActionName("Dummy"), nullptr);
            MR::hideModelAndOnCalcAnim(this);
            MR::deleteEffect(this, "TicoFatLight");
            if (_94) {
                MR::stopBck(_94);
                _94->kill();
            }

            if (_98) {
                MR::stopBck(_98);
                _98->kill();
            }

            MR::openWipeWhiteFade(60);
        }

        if (!MR::isWipeActive()) {
            setNerve(&NrvTicoFat::TicoFatNrvInfo::sInstance);
        }
    }
}

void TicoFat::exeInfo() {
    if (MR::isFirstStep(this)) {
        MR::startSubBGM("BGM_APPEAR_PLANET", false);
        appearInformation();
    }

    if (MR::testCorePadTriggerA(0)) {
        MR::startSystemSE("SE_SY_TALK_OK", -1, -1);
        MR::disappearInformationMessage();
        setNerve(&NrvTicoFat::TicoFatNrvAfter::sInstance);
    }
}

void TicoFat::exeAfter() {
    if (MR::isFirstStep(this)) {
        
    }

    if (MR::isGreaterEqualStep(this, 60)) {
        MR::endMultiActorCamera(this, mCameraInfo, "飛行", false, -1);
        MR::endDemo(this, "変身");
        disappear(true);
        kill();
    }
}

TicoFat::~TicoFat() {
    
}
