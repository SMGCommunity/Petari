#include "Game/NPC/TicoRail.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace {
    static const s32 sStepForWait = 60;
    static const s32 sStepForLookAroundL = 40;
    static const s32 sStepForLookAroundLR = 120;
    static const s32 sStepForLookAroundLRL = 160;
    static const s32 sStepForMove = 500;
    // static const s32 sStepForMoveAccel = _;
    static const s32 sStepForTalk = 320;
    // static const f32 sRailSpeedMax = _;
    // static const f32 sRailSpeedTalk = _;
    static const f32 sRailSpeedGoodBye = 1.5f;
    static const f32 sTurnRate = 1.2f;
    static const f32 sTalkTerritoryY = 30.0f;
    // static const s32 sRandomMax = _;
};  // namespace

namespace NrvTicoRail {
    NEW_NERVE(TicoRailNrvWait, TicoRail, Wait);
    NEW_NERVE(TicoRailNrvLookAround, TicoRail, LookAround);
    NEW_NERVE(TicoRailNrvMoveSign, TicoRail, MoveSign);
    NEW_NERVE(TicoRailNrvMoveSignAndTurn, TicoRail, MoveSign);
    NEW_NERVE(TicoRailNrvMove, TicoRail, Move);
    NEW_NERVE(TicoRailNrvStop, TicoRail, Stop);
    NEW_NERVE(TicoRailNrvTalkStart, TicoRail, TalkStart);
    NEW_NERVE(TicoRailNrvTalk, TicoRail, Talk);
    NEW_NERVE(TicoRailNrvTalkCancel, TicoRail, TalkCancel);
    NEW_NERVE(TicoRailNrvGoodBye, TicoRail, GoodBye);
};  // namespace NrvTicoRail

TicoRail::TicoRail(const char* pName) : LiveActor(pName), _8C(0.0f, 0.0f, 1.0f), _98(nullptr), mLodCtrl(nullptr) {}

void TicoRail::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Tico", nullptr, false);
    MR::connectToSceneNpc(this);
    initHitSensor(1);
    MR::addHitSensorNpc(this, "body", 8, 50.0f, TVec3f(0.0f, 50.0f, 0.0f));
    MR::initLightCtrl(this);
    initEffectKeeper(0, nullptr, false);
    MR::initShadowFromCSV(this, "Shadow");
    initRailRider(rIter);
    MR::moveCoordAndTransToNearestRailPos(this);
    _8C.set< f32 >(MR::getRailDirection(this));
    mLodCtrl = MR::createLodCtrlNPC(this, rIter);
    s32 arg0 = 0;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    s32 color = arg0;
    MR::startBrk(this, "ColorChange");
    MR::setBrkFrameAndStop(this, color);
    AstroDemoFunction::tryRegisterGrandStarReturnAndSimpleCast(this, rIter);
    AstroDemoFunction::tryRegisterDemo(this, "バトラーグリーンドライバ説明", rIter);
    s32 rand = MR::getRandom(0l, 2l);

    if (rand == 0) {
        initNerve(&NrvTicoRail::TicoRailNrvWait::sInstance);
    } else if (rand == 1) {
        initNerve(&NrvTicoRail::TicoRailNrvMove::sInstance);
    }

    makeActorAppeared();
}

void TicoRail::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Turn", nullptr);
    }

    if (isGreaterEqualStepAndRandom(sStepForWait)) {
        setNerve(&NrvTicoRail::TicoRailNrvLookAround::sInstance);
    }
}

void TicoRail::exeLookAround() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Turn", nullptr);
    }

    TVec3f up;
    MR::calcUpVec(&up, this);

    f32 rotate = 0.0f;

    if (MR::isLessStep(this, sStepForLookAroundL)) {
        rotate = sTurnRate;
    } else if (MR::isLessStep(this, sStepForLookAroundLR)) {
        rotate = -sTurnRate;
    } else if (MR::isLessStep(this, sStepForLookAroundLRL)) {
        rotate = sTurnRate;
    }

    MR::rotateVecDegree(&_8C, up, rotate);

    if (MR::isStep(this, 160)) {
        if (MR::getRandom(0l, 2l) != 0) {
            setNerve(&NrvTicoRail::TicoRailNrvMoveSign::sInstance);
        } else {
            setNerve(&NrvTicoRail::TicoRailNrvMoveSignAndTurn::sInstance);
        }
    }
}

void TicoRail::exeMoveSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Spin", nullptr);

        if (isNerve(&NrvTicoRail::TicoRailNrvMoveSignAndTurn::sInstance)) {
            MR::reverseRailDirection(this);
        }
    }

    f32 rate = MR::calcNerveRate(this, MR::getBckFrameMax(this));
    const TVec3f* railDirection = &MR::getRailDirection(this);
    TVec3f reversedDir;
    JMathInlineVEC::PSVECNegate(railDirection, &reversedDir);
    MR::blendVec(&_8C, reversedDir, MR::getRailDirection(this), rate);

    if (MR::isBckStopped(this)) {
        setNerve(&NrvTicoRail::TicoRailNrvMove::sInstance);
    }
}

void TicoRail::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Wait", nullptr);
    }

    MR::moveCoordAndFollowTrans(this, MR::calcNerveValue(this, 0, 200.0f, 15.0f));
    _8C.set< f32 >(MR::getRailDirection(this));

    if (isGreaterEqualStepAndRandom(sStepForMove)) {
        setNerve(&NrvTicoRail::TicoRailNrvStop::sInstance);
    }
}

void TicoRail::exeStop() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Spin", nullptr);
    }

    MR::moveCoordAndFollowTrans(this, MR::calcNerveValue(this, MR::getBckFrameMax(this), 15.0f, 0.0f));

    if (MR::isBckStopped(this)) {
        setNerve(&NrvTicoRail::TicoRailNrvWait::sInstance);
    }
}

void TicoRail::exeTalkStart() {
    TVec3f diff;
    JMathInlineVEC::PSVECSubtract(&mPosition, &_98->mPosition, &diff);
    MR::normalize(&diff);

    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Spin", nullptr);
        TVec3f v14(MR::getRailDirection(this));

        if (diff.dot(v14) > 0.0f) {
            MR::reverseRailDirection(this);
        }
    }

    MR::moveCoordAndFollowTrans(this, 2.0f);
    MR::blendVec(&_8C, MR::getRailDirection(this), diff, MR::calcNerveRate(this, MR::getBckFrameMax(this)));

    if (MR::isBckStopped(this)) {
        setNerve(&NrvTicoRail::TicoRailNrvTalk::sInstance);
    }
}

void TicoRail::exeTalk() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Talk", nullptr);
    }

    if (!MR::isBckPlaying(this, "Reaction") && MR::getRandom(0l, 60l) == 0) {
        MR::startBckWithInterpole(this, "Reaction", 5);
    }

    if (MR::isBckOneTimeAndStopped(this)) {
        MR::startBck(this, "Talk", nullptr);
    }

    if (MR::isStep(this, sStepForTalk)) {
        setNerve(&NrvTicoRail::TicoRailNrvGoodBye::sInstance);
    }
}

void TicoRail::exeTalkCancel() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Spin", nullptr);
    }

    MR::moveCoordAndFollowTrans(this, 15.0f);
    _8C.set< f32 >(MR::getRailDirection(this));

    if (MR::isBckStopped(this)) {
        setNerve(&NrvTicoRail::TicoRailNrvMove::sInstance);
    }
}

void TicoRail::exeGoodBye() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "CallBack", nullptr);

        TVec3f v3(MR::getRailDirection(this));

        if (_8C.dot(v3) > 0.0f) {
            MR::reverseRailDirection(this);
        }
    }

    MR::moveCoordAndFollowTrans(this, sRailSpeedGoodBye);

    if (MR::isBckLooped(this)) {
        _98 = nullptr;
        setNerve(&NrvTicoRail::TicoRailNrvMoveSign::sInstance);
    }
}

bool TicoRail::isGreaterEqualStepAndRandom(s32 step) const {
    if (MR::isGreaterEqualStep(this, step + 300)) {
        return true;
    }

    return MR::isGreaterEqualStep(this, step) && MR::getRandom(0l, 300l) == 0;
}

void TicoRail::kill() {
    LiveActor::kill();
    mLodCtrl->invalidate();
}

void TicoRail::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, _8C);
    MR::setBaseTRMtx(this, mtx);
}

void TicoRail::control() {
    mLodCtrl->update();
}

void TicoRail::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        MR::sendMsgPush(pReceiver, pSender);
    } else if (MR::isSensorNpc(pReceiver)) {
        bool v6 = isNerve(&NrvTicoRail::TicoRailNrvTalkStart::sInstance) || isNerve(&NrvTicoRail::TicoRailNrvTalk::sInstance) ||
                  isNerve(&NrvTicoRail::TicoRailNrvTalkCancel::sInstance) || isNerve(&NrvTicoRail::TicoRailNrvGoodBye::sInstance);

        if (v6) {
            return;
        }

        if (MR::sendArbitraryMsg(ACTMES_TICO_RAIL_TOUCH, pReceiver, pSender)) {
            _98 = pReceiver->mHost;
            setNerve(&NrvTicoRail::TicoRailNrvTalkStart::sInstance);
        } else if (!MR::isExistRail(pReceiver->mHost) || (MR::isRailGoingToEnd(this) != MR::isRailGoingToEnd(pReceiver->mHost))) {
            setNerve(&NrvTicoRail::TicoRailNrvTalkCancel::sInstance);
        }
    }
}

bool TicoRail::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg != ACTMES_TICO_RAIL_TOUCH) {
        return false;
    }

    bool v8 = isNerve(&NrvTicoRail::TicoRailNrvTalkStart::sInstance) || isNerve(&NrvTicoRail::TicoRailNrvTalk::sInstance) ||
              isNerve(&NrvTicoRail::TicoRailNrvTalkCancel::sInstance) || isNerve(&NrvTicoRail::TicoRailNrvGoodBye::sInstance);

    if (v8) {
        return false;
    }

    if (!isSameRailActor(pReceiver->mHost)) {
        return false;
    }

    if (MR::getRandom(0, 2l) == 0) {
        return false;
    }

    if (MR::calcDistanceVertical(this, pSender->mHost->mPosition) > sTalkTerritoryY) {
        return false;
    }

    _98 = pSender->mHost;
    setNerve(&NrvTicoRail::TicoRailNrvTalkStart::sInstance);

    return true;
}

bool TicoRail::isSameRailActor(const LiveActor* pActor) const {
    if (!MR::isExistRail(pActor)) {
        return false;
    }

    return MR::getRailPointPosStart(this) == MR::getRailPointPosStart(pActor) && MR::getRailPointPosEnd(this) == MR::getRailPointPosEnd(pActor);
}
