#include "Game/NPC/TicoRail.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace NrvTicoRail {
    NEW_NERVE(TicoRailNrvWait, TicoRail, Wait);
    NEW_NERVE(TicoRailNrvLookAround, TicoRail, Wait);
    NEW_NERVE(TicoRailNrvMoveSign, TicoRail, Wait);
    NEW_NERVE(TicoRailNrvMoveSignAndTurn, TicoRail, Wait);
    NEW_NERVE(TicoRailNrvMove, TicoRail, Wait);
    NEW_NERVE(TicoRailNrvStop, TicoRail, Wait);
    NEW_NERVE(TicoRailNrvTalkStart, TicoRail, Wait);
    NEW_NERVE(TicoRailNrvTalk, TicoRail, Wait);
    NEW_NERVE(TicoRailNrvTalkCancel, TicoRail, Wait);
    NEW_NERVE(TicoRailNrvGoodBye, TicoRail, Wait);
};

TicoRail::TicoRail(const char *pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = 0.0f;
    _8C.z = 0.0f;
    _98 = 0;
    mLodCtrl = nullptr;
}

void TicoRail::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Tico", nullptr, false);
    MR::connectToSceneNpc(this);
    initHitSensor(1);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 50.0f;
    offs.z = 0.0f;
    MR::addHitSensorNpc(this, "body", 8, 50.0f, offs);
    MR::initLightCtrl(this);
    initEffectKeeper(0, nullptr, false);
    MR::initShadowFromCSV(this, "Shadow");
    initRailRider(rIter);
    MR::moveCoordAndTransToNearestRailPos(this);
    _8C.set<f32>(MR::getRailDirection(this));
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
    }
    else if (rand == 1) {
        initNerve(&NrvTicoRail::TicoRailNrvMove::sInstance);
    }
    makeActorAppeared();
}

void TicoRail::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Turn", nullptr);
    }

    if (isGreaterEqualStepAndRandom(60)) {
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
    if (MR::isLessStep(this, 40)) {
        rotate = 1.2f;
    }
    else if (MR::isLessStep(this, 120)) {
        rotate = -1.2f;
    }
    else if (MR::isLessStep(this, 160)) {
        rotate = 1.2f;
    }

    MR::rotateVecDegree(&_8C, up, rotate);
    if (MR::isStep(this, 160)) {
        if (MR::getRandom(0l, 2l) == 0) {
            setNerve(&NrvTicoRail::TicoRailNrvMoveSign::sInstance);
        }
        else {
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
    _8C.set<f32>(MR::getRailDirection(this));
    if (isGreaterEqualStepAndRandom(500)) {
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
    JMathInlineVEC::PSVECSubtract(&_98->mPosition, &mPosition, &diff);
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

    if (MR::isStep(this, 320)) {
        setNerve(&NrvTicoRail::TicoRailNrvGoodBye::sInstance);
    }
}

void TicoRail::exeTalkCancel() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Spin", nullptr);
    }

    MR::moveCoordAndFollowTrans(this, 15.0f);
    _8C.set<f32>(MR::getRailDirection(this));
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

    MR::moveCoordAndFollowTrans(this, 1.5f);
    if (MR::isBckLooped(this)) {
        _98 = nullptr;
        setNerve(&NrvTicoRail::TicoRailNrvMoveSign::sInstance);
    }
}

bool TicoRail::isGreaterEqualStepAndRandom(s32 step) const {
    if (MR::isGreaterEqualStep(this, step + 300)) {
        return true;
    }

    bool ret = false;

    if (MR::isGreaterEqualStep(this, step) && MR::getRandom(0l, 300l) == 0) {
        ret = true;
    }

    return ret;
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

void TicoRail::attackSensor(HitSensor *a2, HitSensor *a3) {
    if (MR::isSensorPlayer(a3)) {
        MR::sendMsgPush(a3, a2);
    }
    else if (MR::isSensorNpc(a3)) {
        bool v6 = false;
        if (isNerve(&NrvTicoRail::TicoRailNrvTalkStart::sInstance) || isNerve(&NrvTicoRail::TicoRailNrvTalk::sInstance)
            || isNerve(&NrvTicoRail::TicoRailNrvTalkCancel::sInstance) || isNerve(&NrvTicoRail::TicoRailNrvGoodBye::sInstance)) {
        
            v6 = true;
        }

        if (!v6) {
            if (MR::sendArbitraryMsg(195, a3, a2)) {
                _98 = a3->mActor;
                setNerve(&NrvTicoRail::TicoRailNrvTalkStart::sInstance);
            }
            else if (!MR::isExistRail(a3->mActor) || (MR::isRailGoingToEnd(this) != MR::isRailGoingToEnd(a3->mActor))) {
                setNerve(&NrvTicoRail::TicoRailNrvTalkCancel::sInstance);
            }
        }
    }
}


bool TicoRail::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (msg != 195) {
        return false;
    }

    bool v8 = false;
    if (isNerve(&NrvTicoRail::TicoRailNrvTalkStart::sInstance) || isNerve(&NrvTicoRail::TicoRailNrvTalk::sInstance)
        || isNerve(&NrvTicoRail::TicoRailNrvTalkCancel::sInstance) || isNerve(&NrvTicoRail::TicoRailNrvGoodBye::sInstance)) {
    
        v8 = true;
    }

    if (v8) {
        return false;
    }

    if (!isSameRailActor(a3->mActor)) {
        return false;
    }

    if (MR::getRandom(0l, 2l) == 0) {
        return false;
    }

    if (MR::calcDistanceVertical(this, a2->mActor->mPosition) > 30.0f) {
        return false;
    }

    _98 = a2->mActor;
    setNerve(&NrvTicoRail::TicoRailNrvTalkStart::sInstance);
    return true;
}

bool TicoRail::isSameRailActor(const LiveActor *pActor) const {
    if (!MR::isExistRail(pActor)) {
        return false;
    }

    bool ret = false;
    if (MR::getRailPointPosStart(this) == MR::getRailPointPosStart(pActor)) {
        if (MR::getRailPointPosEnd(this) == MR::getRailPointPosEnd(pActor)) {
            ret = true;
        }
    }

    return ret;
}

TicoRail::~TicoRail() {
    
}
