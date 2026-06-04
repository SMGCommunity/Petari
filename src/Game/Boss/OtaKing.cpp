#include "Game/Boss/OtaKing.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/Boss/OtaKingLongFoot.hpp"
#include "Game/Boss/OtaKingMagma.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/CocoNutBall.hpp"
#include "Game/Enemy/FireBall.hpp"
#include "Game/Enemy/FireBubble.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/MapObj/CocoNut.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"

#include <cstdio>

namespace NrvOtaKing {
    NEW_NERVE(OtaKingNrvWaitOnSwitch, OtaKing, WaitOnSwitch)
    NEW_NERVE(OtaKingNrvAppearDemo, OtaKing, AppearDemo)
    NEW_NERVE(OtaKingNrvWait, OtaKing, Wait);
    NEW_NERVE(OtaKingNrvThrowCocoNutWait, OtaKing, ThrowCocoNutWait);
    NEW_NERVE(OtaKingNrvThrowCocoNut, OtaKing, ThrowCocoNut);
    NEW_NERVE(OtaKingNrvThrowFireBallWait, OtaKing, ThrowFireBallWait);
    NEW_NERVE(OtaKingNrvThrowFireBall, OtaKing, ThrowFireBall);
    NEW_NERVE(OtaKingNrvDamage, OtaKing, Damage);
    NEW_NERVE(OtaKingNrvPowerUp, OtaKing, PowerUp);
    NEW_NERVE(OtaKingNrvDown, OtaKing, Down);
    NEW_NERVE(OtaKingNrvDownDemo, OtaKing, DownDemo);
    NEW_NERVE(OtaKingNrvAppearStar, OtaKing, AppearStar);
    NEW_NERVE(OtaKingNrvDead, OtaKing, Dead);
    NEW_NERVE(OtaKingNrvHitBackStart, OtaKing, HitBackStart);
    NEW_NERVE(OtaKingNrvHitBack, OtaKing, HitBack);
    NEW_NERVE(OtaKingNrvNearAttack, OtaKing, NearAttack);
    NEW_NERVE(OtaKingNrvBubbleAttack, OtaKing, BubbleAttack);
    NEW_NERVE(OtaKingNrvWaitStartDemo, OtaKing, WaitStartDemo);
}  // namespace NrvOtaKing

namespace {
    const Vec cLongFootScale = {1.0f, 0.9f, 0.8f};
    const Vec cBubbleAppearVelocity = {0.0f, 37.5f, 15.0f};
    const Vec cSensorOffset = {0.0f, 150.0f, 0.0f};
    const u32 cLongFootDemoBckStep[] = {3, 8, 14, 18};
    const Vec cAppearDemoRotate = {0.0f, 30.0f, 0.0f};
    const Vec cDownDemoRotate = {0.0f, 43.0f, 0.0f};
    const Vec cDownDemoCocoNutPos = {0.0f, 630.0f, 250.0f};
    const f32 cFireBallThrowNum = 3.0f;
}  // namespace

OtaKing::OtaKing(const char* pName)
    : LiveActor(pName), _A4(nullptr), _D8(nullptr), _DC(nullptr), _E0(nullptr), _E4(nullptr), _E8(0), _EC(0), _f0(0.0f), _f4(nullptr), _f8(gZeroVec),
      _104(nullptr), _108(nullptr), _10C(nullptr), _150(false), _154(nullptr) {
    MR::zeroMemory(_8C, sizeof(_8C));
    MR::zeroMemory(_94, sizeof(_94));
    _A8.identity();
    MR::zeroMemory(_110, sizeof(_110) + sizeof(_130));
}

void OtaKing::init(const JMapInfoIter& rIter) {
    initMapToolInfo(rIter);
    initModel(rIter);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initSensor();
    initEffectKeeper(0, "OtaKing", false);
    MR::setClippingTypeSphere(this, 3000.0f);
    initSound(8, false);
    _f4 = new AudAnmSoundObject(&_f8, 4, MR::getCurrentHeap());
    _10C = MR::createActorCameraInfo(rIter);
    MR::initAnimCamera(this, _10C, "Appear");
    MR::initAnimCamera(this, _10C, "Down");
    MR::declarePowerStar(this);
    MR::declareStarPiece(this, 24);
    MR::createCenterScreenBlur();

    if (MR::isValidSwitchA(this)) {
        MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &startAppearDemo));
    }

    MR::tryRegisterDemoCast(this, rIter);
    _154 = new AnimScaleController(nullptr);
    initNerve(&NrvOtaKing::OtaKingNrvWait::sInstance);
    makeActorAppeared();
}

void OtaKing::movement() {
    LiveActor::movement();
    MR::copyJointPos(this, "Crown", &_f8);
    _f4->process();
}

void OtaKing::makeActorAppeared() {
    _E8 = 0;
    LiveActor::makeActorAppeared();

    MR::startBrk(this, "Wait");
    MR::startBpk(this, "Wait");

    if (MR::isValidSwitchA(this)) {
        setNerve(&NrvOtaKing::OtaKingNrvWaitOnSwitch::sInstance);
    } else {
        setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
    }
}

void OtaKing::startAppearDemo() {
    if (isNerve(&NrvOtaKing::OtaKingNrvWaitOnSwitch::sInstance)) {
        invalidateClippingAndStartDemo("出現", &NrvOtaKing::OtaKingNrvAppearDemo::sInstance, &NrvOtaKing::OtaKingNrvWaitStartDemo::sInstance);
    }
}

void OtaKing::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    NameObjArchiveListCollector* pList = pArchiveList;
    bool isLv2 = false;
    MR::getJMapInfoArg1NoInit(rIter, &isLv2);
    bool isLv2Flag = isLv2;

    pList->addArchive(CocoNut::getModelName());
    pList->addArchive("FireBall");
    pList->addArchive("FireBubble");
    pList->addArchive("OtaKingMagma");
    pList->addArchive("OtaKingMagmaBloom");

    if (!isLv2Flag) {
        pList->addArchive("OtaKing");
        pList->addArchive("OtaKingFoot");
        pList->addArchive("OtaKingLongFoot");
    } else {
        pList->addArchive("OtaKingLv2");
        pList->addArchive("OtaKingFootLv2");
    }
}

void OtaKing::control() {
    if (isNerve(&NrvOtaKing::OtaKingNrvDown::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvAppearStar::sInstance) ||
        isNerve(&NrvOtaKing::OtaKingNrvDead::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvAppearDemo::sInstance) ||
        isNerve(&NrvOtaKing::OtaKingNrvDownDemo::sInstance)) {
        _f0 = 0.0f;
    } else {
        dirToPlayer();
    }
    _154->updateNerve();
}

void OtaKing::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    MR::setBaseScale(this, _154->_C.mult(mScale));
}

bool OtaKing::isSensorBodyOrFace(HitSensor* pSensor) {
    return pSensor == getSensor("body") || pSensor == getSensor("face");
}

void OtaKing::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && isSensorBodyOrFace(pSender)) {
        if (isNerveValidNearAttack() && MR::sendMsgEnemyAttackMaximum(pReceiver, pSender)) {
            MR::sendMsgPush(pReceiver, pSender);
            setNerve(&NrvOtaKing::OtaKingNrvNearAttack::sInstance);
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool OtaKing::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isSensorBodyOrFace(pReceiver) && MR::isMsgStarPieceReflect(msg)) {
        _154->startHitReaction();
        return true;
    }

    return false;
}

bool OtaKing::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!pSender->isType(ATYPE_COCO_NUT)) {
        return false;
    }

    CocoNutBall* rallyBall = reinterpret_cast< CocoNutBall* >(pSender->mHost);

    s32 a1 = _E8;

    if (_150) {
        a1 = MR::max(_E8, 1);
    }
    bool b1 = rallyBall->_9C <= a1;

    bool isReceiverBody = pReceiver == getSensor("body");

    if (isReceiverBody) {
        if (b1) {
            if (!isNerve(&NrvOtaKing::OtaKingNrvHitBack::sInstance)) {
                setNerve(&NrvOtaKing::OtaKingNrvHitBack::sInstance);
            }

            rallyBall->hitBackToPlayer();
        } else {
            if (MR::isPlayingStageBgm()) {
                MR::startSystemME("ME_RALLY_COMBO_LAST");
            }

            damage();
            return true;
        }
    } else {
        if (b1 && !isNerve(&NrvOtaKing::OtaKingNrvHitBackStart::sInstance)) {
            setNerve(&NrvOtaKing::OtaKingNrvHitBackStart::sInstance);
        }
    }

    return false;
}

bool OtaKing::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    switch (msg)
    case ACTMES_RUSH_END:
        if (pSender->isType(ATYPE_COCO_NUT)) {
            if (!isDamageNerve()) {
                if (isNerve(&NrvOtaKing::OtaKingNrvWait::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvThrowCocoNutWait::sInstance) ||
                    isNerve(&NrvOtaKing::OtaKingNrvHitBackStart::sInstance)) {
                    if (isValidBubbleAttack()) {
                        setNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
                    } else {
                        _E8 > 0 && isValidThrowFireBall() ? setNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance) :
                                                            setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
                    }
                    return true;
                } else if (isNerve(&NrvOtaKing::OtaKingNrvThrowCocoNut::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvHitBack::sInstance)) {
                    _EC = 2;
                    return true;
                }
            }
        }
    return false;
}

void OtaKing::initMapToolInfo(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchWriteA(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);

    bool arg1 = false;

    MR::getJMapInfoArg1NoInit(rIter, &arg1);
    _150 = arg1;
}

void OtaKing::initModel(const JMapInfoIter& rIter) {
    initModelManagerWithAnm(_150 ? "OtaKing" : "OtaKingLv2", nullptr, false);

    for (int i = 0; i < 2; i++) {
        _8C[i] = new PartsModel(this, "前足モデル", _150 ? "OtaKingFoot" : "OtaKingFootLv2", nullptr, 18, false);
        _8C[i]->mPosition.set(mPosition);
        _8C[i]->initWithoutIter();
        MR::initLightCtrl(_8C[i]);
    }

    _8C[0]->initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), nullptr);
    _8C[1]->initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), nullptr);

    initLongFoot(rIter);

    _D8 = new CocoNutBall[3];
    for (int i = 0; i < 3; i++) {
        _D8[i].initWithoutIter();
        _D8[i]._8C = this;
        _D8[i]._BE = true;
        _D8[i]._D8 = true;
        _D8[i]._D4 = 5000.0f;

        float arg0;
        if (MR::getJMapInfoArg0NoInit(rIter, &arg0)) {
            _D8[i]._C0 = arg0;
        }

        float arg2;
        if (MR::getJMapInfoArg2NoInit(rIter, &arg2)) {
            _D8[i]._C4 = arg2;
        }
    }

    _DC = new FireBall[6];

    for (int i = 0; i < 6; i++) {
        _DC[i].initWithoutIter();
        _DC[i]._8C = this;
    }

    _E0 = new FireBubble[6];
    for (int i = 0; i < 6; i++) {
        _E0[i].initWithoutIter();
    }

    _E4 = new FixedPosition(this, "Lip", TVec3f(-50.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
    _104 = new FixedPosition(this, TVec3f(0.0f, 10.0f, 500.0f), TVec3f(0.0f, 0.0f, 0.0f));

    _A4 = new OtaKingMagma(this, -1);
    _A4->initWithoutIter();
    _108 = new CocoNutBall;
    _108->mScale.set(1.5f);
    _108->initWithoutIter();
}

void OtaKing::initSensor() {
    initHitSensor(3);
    MR::addHitSensorEnemy(this, "body", 16, 400.0f, ::cSensorOffset);
    MR::addHitSensorAtJointEnemy(this, "face", "Face", 16, 400.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEnemy(this, "search", 16, 1500.0f, ::cSensorOffset);
}

void OtaKing::dirToPlayer() {
    TVec3f vec;
    vec.sub(*MR::getPlayerPos(), mPosition);

    f32 angle = JMAATan2(vec.x, vec.z);

    angle = MR::repeat(MR::toDegree(angle), mRotation.y - 180.0f, 360.0f);

    f32 angleminroty = angle - mRotation.y;

    if ((0.0f < angleminroty && _f0 < angleminroty) || (angleminroty < 0.0f && angleminroty < _f0)) {
        if (angleminroty * _f0 < 0.0f) {
            _f0 = 0.0f;
        }
        mRotation.y += _f0;

        if (0.0f < angleminroty) {
            _f0 = MR::max(_f0 + 0.03f, 1.0f);
        } else {
            _f0 = MR::min(_f0 - 0.03f, -1.0f);
        }

    } else {
        mRotation.y = angle;
        _f0 = 0.0f;
    }

    MR::repeatDegree(&mRotation.y);
}

CocoNutBall* OtaKing::getDisappearedCocoNut() {
    for (int i = 0; i < 3; i++) {
        if (MR::isDead(&_D8[i])) {
            return &_D8[i];
        }
    }
    return nullptr;
}

FireBall* OtaKing::getDisappearedFireBall() {
    for (int i = 0; i < 6; i++) {
        if (MR::isDead(&_DC[i])) {
            return &_DC[i];
        }
    }
    return nullptr;
}

FireBubble* OtaKing::getDisappearedBubble() {
    for (int i = 0; i < 6; i++) {
        if (MR::isDead(&_E0[i])) {
            return &_E0[i];
        }
    }
    return nullptr;
}

s32 OtaKing::getDisappearedCocoNutNum() const {
    s32 dissappearedNum = 0;
    for (int i = 0; i < 3; i++) {
        if (MR::isDead(&_D8[i])) {
            dissappearedNum++;
        }
    }
    return dissappearedNum;
}

s32 OtaKing::getDisappearedFireBallNum() const {
    s32 dissappearedNum = 0;
    for (int i = 0; i < 6; i++) {
        if (MR::isDead(&_DC[i])) {
            dissappearedNum++;
        }
    }
    return dissappearedNum;
}

s32 OtaKing::getDisappearedFireBubbleNum() const {
    s32 dissappearedNum = 0;
    for (int i = 0; i < 6; i++) {
        if (MR::isDead(&_E0[i])) {
            dissappearedNum++;
        }
    }
    return dissappearedNum;
}

bool OtaKing::isValidThrowCocoNut() const {
    if (getDisappearedCocoNutNum() == 0) {
        return false;
    }

    f32 distToPlayer = MR::calcDistanceToPlayer(this);
    if (4000.0f < distToPlayer) {
        return false;
    }

    for (int i = 0; i < 3; i++) {
        CocoNutBall* currentCocoNut = &_D8[i];
        if (!MR::isDead(currentCocoNut) && MR::isNear(this, currentCocoNut, distToPlayer + 500.0f)) {
            return false;
        }
    }

    return true;
}

bool OtaKing::isValidThrowFireBall() const {
    return cFireBallThrowNum <= getDisappearedFireBallNum() && MR::calcDistanceToPlayer(this) < 4000.0f && (!_150 || getDisappearedCocoNutNum() == 3);
}

bool OtaKing::isValidBubbleAttack() const {
    if (getDisappearedFireBubbleNum() != 6) {
        return false;
    }

    if (_150) {
        return _E8 > 0;
    } else {
        return isOneHP();
    }
}

// fabricated inline used in multiple functions
bool OtaKing::isDamageNerve() {
    return isNerve(&NrvOtaKing::OtaKingNrvDamage::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvPowerUp::sInstance) ||
           isNerve(&NrvOtaKing::OtaKingNrvDown::sInstance);
}

void OtaKing::damage() {
    if (!isDamageNerve()) {
        MR::deleteEffectAll(this);

        _E8++;

        if (_E8 < 3) {
            MR::startSound(this, "SE_BV_OTAKING_DAMAGE", -1, -1);
            MR::startSound(this, "SE_BM_OTAKING_DAMAGE", -1, -1);
            // why use modulo here, we already know its less than 3 and positive????
            switch (_E8 % 3) {
            case 0:
                MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_3", -1, -1);
                break;
            case 1:
                MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_1", -1, -1);
                break;
            case 2:
                MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_2", -1, -1);
            }

            if (isOneHP()) {
                setNerve(&NrvOtaKing::OtaKingNrvPowerUp::sInstance);
            } else {
                setNerve(&NrvOtaKing::OtaKingNrvDamage::sInstance);
            }
        } else {
            setNerve(&NrvOtaKing::OtaKingNrvDown::sInstance);
        }
    }
}

void OtaKing::throwCocoNut() {
    CocoNutBall* cocoNut = getDisappearedCocoNut();

    _E4->calc();
    TVec3f trans;
    _E4->copyTrans(&trans);

    TVec3f vec;
    vec.sub(*MR::getPlayerPos(), mPosition);

    f32 angle = JMAATan2(vec.x, vec.z);

    angle = mRotation.y - MR::repeat(MR::toDegree(angle), mRotation.y - 180.0f, 360.0f);
    f32 random = MR::getRandom(2.5f, 5.0f);

    cocoNut->appearAndThrow(trans, angle + (_EC % 2 == _E8 % 2 ? random : -random));
    _EC++;
}

void OtaKing::throwFireBall() {
    _E4->calc();

    TVec3f trans;
    _E4->copyTrans(&trans);

    TVec3f vec;
    vec.sub(*MR::getPlayerPos(), mPosition);

    f32 angle = JMAATan2(vec.x, vec.z);
    angle = mRotation.y - MR::repeat(MR::toDegree(angle), mRotation.y - 180.0f, 360.0f);

    for (int i = 0; i < cFireBallThrowNum; i++) {
        FireBall* currentFireBall = getDisappearedFireBall();

        f32 throwAngle = angle;
        if (i != 0) {
            f32 angleOffset;
            if ((i % 2) == 0) {
                angleOffset = 30.0f * ((i + 1) / 2);
            } else {
                angleOffset = -30.0f * ((i + 1) / 2);
            }
            throwAngle += angleOffset;
        }

        currentFireBall->appearAndThrow(trans, 15.0f, throwAngle);
    }
}

void OtaKing::startBckWaitIfNotPlaying() {
    MR::tryStartBck(this, "Wait", nullptr);

    for (int i = 0; i < 2; i++) {
        MR::tryStartBck(_8C[i], i == 0 ? "WaitR" : "WaitL", nullptr);
    }
}

void OtaKing::startBckWithFrontFoot(const char* bckName) {
    MR::startBck(this, bckName, nullptr);
    snprintf(_110, sizeof(_110), "%sR", bckName);
    MR::startBck(_8C[0], _110, nullptr);
    snprintf(_130, sizeof(_130), "%sL", bckName);
    MR::startBck(_8C[1], _130, nullptr);
}

bool OtaKing::isNerveValidNearAttack() const {
    return isNerve(&NrvOtaKing::OtaKingNrvWait::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvThrowCocoNutWait::sInstance) ||
           isNerve(&NrvOtaKing::OtaKingNrvThrowCocoNut::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance) ||
           isNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
}

void OtaKing::appearBubble() {
    _104->calc();

    TVec3f trans;

    _104->copyTrans(&trans);

    for (int i = 0; i < 6; i++) {
        FireBubble* currentBubble = getDisappearedBubble();

        if (currentBubble != nullptr) {
            TVec3f vec1;
            vec1.sub(trans, mPosition);

            if (MR::normalizeOrZero(&vec1)) {
                vec1.set(0.0f, 0.0f, 1.0f);
            }

            TPos3f rotate;

            rotate.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), MR::toRadian(((i + 0.5f) * 60.0f) + MR::getRandom(-30.0f, 30.0f)));

            rotate.mult33Inline(vec1, vec1);
            currentBubble->appear(trans, vec1, cBubbleAppearVelocity);
        }
    }
}

void OtaKing::startDemo() {
    for (int i = 0; i < 2; i++) {
        MR::requestMovementOn(_8C[i]);
    }

    if (!_150) {
        for (int i = 0; i < 4; i++) {
            MR::requestMovementOn(_94[i]);
        }
    }

    MR::pauseOffEffectAll(this);
    MR::pauseOffEffectAll(_A4);
}

void OtaKing::appearStarPiece() {
    TVec3f appearPos(mPosition);
    appearPos.y += 300.0f;
    MR::appearStarPiece(this, appearPos, _E8 * 8, 60.0f, 50.0f, false);
    MR::startSoundObject(_f4, "SE_OJ_STAR_PIECE_BURST");
}

void OtaKing::initLongFoot(const JMapInfoIter& rIter) {
    if (!_150) {
        for (int i = 0; i < 4; i++) {
            _94[i] = new OtaKingLongFoot(this, cLongFootDemoBckStep[i], "飾り足");
            _94[i]->init(rIter);
            _94[i]->mScale.set(cLongFootScale);
        }

        _A8.identity33();
        _A8.setTrans(mPosition);
        _94[0]->initFixedPosition(_A8, TVec3f(735.0f, 80.0f, -55.0f), TVec3f(-9.0f, 266.0f, 0.0f));
        _94[1]->initFixedPosition(_A8, TVec3f(-959.0f, 130.0f, 0.0f), TVec3f(0.0f, 107.0f, 14.0f));
        _94[2]->initFixedPosition(_A8, TVec3f(0.0f, 43.0f, 884.0f), TVec3f(-8.0f, 159.0f, 0.0f));
        _94[3]->initFixedPosition(_A8, TVec3f(55.0f, 55.0f, -896.0f), TVec3f(-9.0f, -14.0f, 0.0f));
    }
}

bool OtaKing::tryThrowCocoNutOrFireBallIfWait(s32 a1) {
    if (MR::isGreaterStep(this, a1) && isValidThrowCocoNut()) {
        setNerve(&NrvOtaKing::OtaKingNrvThrowCocoNut::sInstance);
        return true;
    }

    if (MR::isGreaterStep(this, a1 * 2) && isValidThrowFireBall()) {
        setNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance);
        return false;
    }

    return false;
}

void OtaKing::exeWaitOnSwitch() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);

        startBckWithFrontFoot("Appear");
        MR::setBckFrameAndStop(this, 0.0f);

        mScale.set(0.001f);

        MR::setBckFrameAndStop(_8C[0], 0.0f);
        MR::hideModel(_8C[0]);
        MR::setBckFrameAndStop(_8C[1], 0.0f);
        MR::hideModel(_8C[1]);

        if (!_150) {
            for (int i = 0; i < 4; i++) {
                _94[i]->hide();
            }
        }
    }
}

void OtaKing::exeAppearDemo() {
    if (MR::isFirstStep(this)) {
        startDemo();
        startBckWithFrontFoot("Appear");
        mScale.set(1.0f);

        MR::showModel(_8C[0]);
        MR::showModel(_8C[1]);

        if (!_150) {
            for (int i = 0; i < 4; i++) {
                _94[i]->startAppearDemo();
            }
        }

        MR::startAnimCameraTargetSelf(this, _10C, "Appear", 0, 1.0f);
        MR::stopStageBGM(0);
        MR::startBossBGM(0);
        mRotation.set(cAppearDemoRotate);
        MR::overlayWithPreviousScreen(2);

        if (_150) {
            MR::forceToFrameCinemaFrame();
        }
    }

    if (MR::isStep(this, 1)) {
        MR::hidePlayer();
    }

    if (MR::isStep(this, 10)) {
        MR::startSound(this, "SE_BM_OTAKING_MAGMA_HITBACK", -1, -1);
    }

    if (MR::isStep(this, 205)) {
        MR::startSound(this, "SE_BM_OTAKING_MAGMA_DAMAGE", -1, -1);
    }

    if (MR::isStep(this, 230)) {
        MR::startSound(this, "SE_BM_OTAKING_MAGMA_HITBACK", -1, -1);
    }

    if (MR::isStep(this, 100)) {
        MR::startSound(this, "SE_BM_OTAKING_MAGMA_HITBACK", -1, -1);
    }

    if (MR::isStep(this, 40)) {
        _A4->attack();
    }

    if (MR::isStep(this, 190)) {
        _A4->appearDemo();
    }

    MR::tryRumblePadVeryWeak(this, 0);

    if (MR::isStep(this, 50)) {
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, 130)) {
        MR::tryRumblePadVeryStrong(this, 0);
        MR::shakeCameraWeak();
    }
    if (MR::isStep(this, 160)) {
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormal();
    }
    if (MR::isStep(this, 200)) {
        MR::shakeCameraStrong();
    }
    if (MR::isGreaterStep(this, 200) && MR::isLessStep(this, 250)) {
        MR::tryRumblePadWeak(this, 0);
    }
    if (MR::isStep(this, 310)) {
        MR::tryRumblePadMiddle(this, 0);
    }

    if (MR::isBckStopped(this)) {
        MR::validateHitSensors(this);
        validateClippingAndEndDemo("出現");
        MR::showPlayer();
        MR::endAnimCamera(this, _10C, "Appear", 0, true);

        if (!_150) {
            for (int i = 0; i < 4; i++) {
                _94[i]->endDemo();
            }
        }

        TVec3f vec;
        vec.sub(*MR::getPlayerPos(), mPosition);

        f32 angle = JMAATan2(vec.x, vec.z);
        mRotation.y = MR::repeatDegree(MR::toDegree(angle));

        setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
    }
}

void OtaKing::exeWait() {
    if (MR::isFirstStep(this)) {
        startBckWaitIfNotPlaying();
    }

    if (tryThrowCocoNutOrFireBallIfWait(75)) {
        _EC = 0;
    }
}

void OtaKing::exeThrowCocoNutWait() {
    if (MR::isFirstStep(this)) {
        startBckWaitIfNotPlaying();
    }

    tryThrowCocoNutOrFireBallIfWait(45);
}

void OtaKing::exeThrowCocoNut() {
    if (MR::isFirstStep(this)) {
        startBckWithFrontFoot("Attack");
        MR::startSound(this, "SE_BV_OTAKING_PRE_RALLYBALL", -1, -1);
    }

    if (MR::isStep(this, 70)) {
        throwCocoNut();
        MR::emitEffect(this, "OtaKingCocoAttack");
        MR::tryRumblePadStrong(this, 0);
    }

    if (MR::isStep(this, 30)) {
        _A4->attack();
    }

    if (MR::isBckStopped(this)) {
        if (isValidBubbleAttack()) {
            setNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
            return;
        }

        if (_E8 > 0) {
            if (isValidThrowFireBall()) {
                setNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance);
            } else {
                setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
            }
        } else if (_EC >= 2) {
            setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
        } else {
            setNerve(&NrvOtaKing::OtaKingNrvThrowCocoNutWait::sInstance);
        }
    }
}

void OtaKing::exeThrowFireBallWait() {
    if (MR::isFirstStep(this)) {
        startBckWaitIfNotPlaying();
    }
    if (MR::isGreaterStep(this, 60) && isValidThrowFireBall()) {
        setNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance);
    }
}

void OtaKing::exeThrowFireBall() {
    if (MR::isFirstStep(this)) {
        startBckWithFrontFoot("FireAttack");
        MR::startSound(this, "SE_BV_OTAKING_PRE_FIREBALL", -1, -1);
    }
    MR::startLevelSound(this, "SE_BM_LV_OTAKING_PRE_FIRE", -1, -1, -1);

    if (MR::isStep(this, 94)) {
        throwFireBall();
        MR::emitEffect(this, "OtaKingFireAttack");
        MR::startSound(this, "SE_BM_OTAKING_FIRE_OUT", -1, -1);
        MR::tryRumblePadStrong(this, 0);
    }

    if (MR::isStep(this, 30)) {
        _A4->fireAttack();
    }

    if (MR::isBckStopped(this)) {
        if (isValidBubbleAttack()) {
            setNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
        } else if (_E8 > 0 && isValidThrowCocoNut()) {
            setNerve(&NrvOtaKing::OtaKingNrvThrowCocoNut::sInstance);
        } else {
            setNerve(&NrvOtaKing::OtaKingNrvWait::sInstance);
        }
    }
}

void OtaKing::exeDamage() {
    if (MR::isFirstStep(this)) {
        startBckWithFrontFoot("Damage");
        MR::tryRumblePadVeryStrong(this, 0);
        appearStarPiece();
        _A4->damage();
    }

    if (MR::isStep(this, 2)) {
        MR::stopScene(_150 ? 6 : 16);
    }

    if (MR::isStep(this, 3)) {
        MR::shakeCameraNormalStrong();
    }

    if (MR::isBckStopped(this)) {
        if (isValidBubbleAttack()) {
            setNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
        } else if (isValidThrowFireBall()) {
            setNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance);
        } else {
            setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
        }
    }
}

void OtaKing::exePowerUp() {
    if (MR::isFirstStep(this)) {
        startBckWithFrontFoot("CrownDamage");
        MR::startBrk(this, "CrownDamage");
        MR::startBpk(this, "CrownDamage");
        MR::tryRumblePadVeryStrong(this, 0);
        appearStarPiece();
        _A4->damage();
    }

    if (MR::isStep(this, 2)) {
        MR::stopScene(_150 ? 6 : 16);
    }

    if (MR::isStep(this, 3)) {
        MR::shakeCameraNormalStrong();
    }

    if (MR::isStep(this, 73)) {
        MR::startSoundObject(_f4, "SE_BM_OTAKING_CROWN_FALL");
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormal();
        _A4->damage();
    }

    if (MR::isGreaterStep(this, 73)) {
        MR::tryRumblePadVeryWeak(this, 0);
    }

    if (MR::isStep(this, 83)) {
        MR::hideMaterial(this, "lambert141_v");
        MR::hideMaterial(this, "Daiya_v");
    }

    if (MR::isStep(this, 175)) {
        _A4->damage();
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, 195)) {
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, 215)) {
        MR::shakeCameraNormal();
    }

    if (MR::isGreaterStep(this, 175) && MR::isLessStep(this, 215)) {
        MR::tryRumblePadWeak(this, 0);
    }

    if (MR::isStep(this, 175)) {
        MR::startCenterScreenBlur(70, 15.0f, 80, 5, 30);
    }

    if (MR::isBckStopped(this)) {
        MR::startBrk(this, "Wait");
        MR::startBpk(this, "Wait");

        MR::showModel(_8C[0]);
        MR::showModel(_8C[1]);

        if (MR::isValidSwitchB(this)) {
            MR::onSwitchB(this);
        }

        if (isValidBubbleAttack()) {
            setNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
            return;
        } else if (isValidThrowFireBall()) {
            setNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance);
            return;
        } else {
            setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
            return;
        }
    }

    if (MR::getPlayerPos()->y < mPosition.y + 600.0f) {
        MR::hideModel(_8C[0]);
        MR::hideModel(_8C[1]);
    } else {
        // unneeded, as showmodel is already called unconditionally
        MR::showModel(_8C[0]);
        MR::showModel(_8C[1]);
    }
}

void OtaKing::exeDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Down", 0);
        MR::startSound(this, "SE_BM_OTAKING_LAST_DAMAGE", -1, -1);
        MR::startSound(this, "SE_BM_OTAKING_DIE", -1, -1);
        MR::startSound(this, "SE_BV_OTAKING_DIE", -1, -1);
        MR::startSystemSE("SE_SY_VS_BOSS_LAST_HIT", -1, -1);
        MR::tryRumblePadVeryStrong(this, 0);
        MR::invalidateHitSensors(this);
        MR::stopStageBGM(60);

        for (int i = 0; i < 3; i++) {
            if (!MR::isDead(&_D8[i])) {
                _D8[i].kill();
            }
        }

        for (int i = 0; i < 6; i++) {
            if (!MR::isDead(&_DC[i])) {
                _DC[i].kill();
            }
        }
        for (int i = 0; i < 6; i++) {
            if (!MR::isDead(&_E0[i])) {
                _E0[i].kill();
            }
        }

        if (!_150) {
            for (int i = 0; i < 4; i++) {
                _94[i]->startDownDemo();
            }
        }
    }

    if (MR::isStep(this, 2)) {
        MR::stopScene(20);
    }

    if (MR::isStep(this, 3)) {
        invalidateClippingAndStartDemo("ダウン", &NrvOtaKing::OtaKingNrvDownDemo::sInstance, &NrvOtaKing::OtaKingNrvWaitStartDemo::sInstance);
    }
}

void OtaKing::exeDownDemo() {
    TPos3f rotate;
    rotate.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), MR::toRadian(cDownDemoRotate.y));
    rotate.setTransInline(mPosition);

    if (MR::isFirstStep(this)) {
        startDemo();
        startBckWithFrontFoot("Down");
        MR::startAnimCameraTargetSelf(this, _10C, "Down", 0, 1.0f);
        mRotation.set(cDownDemoRotate);
        _A4->damage();
        TVec3f demoBreakVec;
        rotate.mult(cDownDemoCocoNutPos, demoBreakVec);
        _108->mScale.set(1.5f);
        _108->demoBreak(demoBreakVec);
        TVec3f starAppearPos(mPosition);
        starAppearPos.y += 100.0f;
        MR::requestAppearPowerStar(this, starAppearPos);
    }

    if (MR::isStep(this, 1)) {
        MR::hidePlayer();
    }

    if (MR::isStep(this, 90)) {
        _A4->down();
    }

    if (MR::isGreaterStep(this, 80)) {
        MR::startLevelSound(this, "SE_BM_LV_OTAKING_SINK", -1, -1, -1);
    }

    mRotation.set(cDownDemoRotate);
    MR::tryRumblePadWeak(this, 0);

    if (MR::isStep(this, 0)) {
        MR::shakeCameraStrong();
    }

    if (MR::isStep(this, 105)) {
        MR::shakeCameraNormal();
        MR::tryRumblePadStrong(this, 0);
    }

    if (MR::isStep(this, 150)) {
        MR::shakeCameraNormal();
        MR::tryRumblePadStrong(this, 0);
    }

    if (MR::isStep(this, 200)) {
        MR::startAfterBossBGM();
    }

    if (MR::isStep(this, 300)) {
        validateClippingAndEndDemo("ダウン");
        setNerve(&NrvOtaKing::OtaKingNrvAppearStar::sInstance);
    }
}

void OtaKing::exeAppearStar() {
    if (MR::isFirstStep(this)) {
        mScale.set(0.001f);
        for (int i = 0; i < 2; i++) {
            _8C[i]->kill();
        }

        if (!_150) {
            for (int i = 0; i < 4; i++) {
                _94[i]->kill();
            }
        }

        getSensor("body")->invalidate();
        getSensor("search")->invalidate();
        
        MR::validateClipping(this);
        MR::pauseOffEffectAll(_A4);
    } else if (MR::isEndPowerStarAppearDemo(this)) {
        MR::showPlayer();
        setNerve(&NrvOtaKing::OtaKingNrvDead::sInstance);
    }
}

void OtaKing::exeDead() {
    if (MR::isFirstStep(this) && MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void OtaKing::exeHitBackStart() {
    if (MR::isFirstStep(this)) {
        startBckWithFrontFoot("HitBackStart");
        MR::deleteEffectAll(this);
    }
}

void OtaKing::exeHitBack() {
    if (MR::isFirstStep(this)) {
        startBckWithFrontFoot("HitBack");
        MR::deleteEffectAll(this);
        MR::tryRumblePadStrong(this, 0);
        _A4->hitBack();
    }

    if (MR::isBckStopped(this)) {
        if (isValidBubbleAttack()) {
            setNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
            return;
        }

        if (_E8 > 0) {
            if (isValidThrowFireBall()) {
                setNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance);
            } else {
                setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
            }
        } else if (_EC >= 2) {
            setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
        } else {
            setNerve(&NrvOtaKing::OtaKingNrvThrowCocoNutWait::sInstance);
        }
    }
}

void OtaKing::exeNearAttack() {
    if (MR::isFirstStep(this)) {
        startBckWithFrontFoot("HitBack");
        MR::deleteEffectAll(this);
        _A4->hitBack();
    }

    if (MR::isStep(this, 20)) {
        _A4->attack();
    }

    if (MR::isBckStopped(this)) {
        if (isValidBubbleAttack()) {
            setNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
        } else {
            setNerve(&NrvOtaKing::OtaKingNrvWait::sInstance);
        }
    }
}

void OtaKing::exeBubbleAttack() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_BV_OTAKING_PRE_FIREBALL", -1, -1);
        startBckWithFrontFoot("BubbleAttack");
    }

    if (MR::isStep(this, 100)) {
        appearBubble();
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, 85)) {
        MR::startSound(this, "SE_BM_OTAKING_MAGMA_BEAT", -1, -1);
        _A4->attack();
    }

    MR::setNerveAtBckStopped(this, &NrvOtaKing::OtaKingNrvWait::sInstance);
}

void OtaKing::exeWaitStartDemo() {
}
