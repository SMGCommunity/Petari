#include "Game/MapObj/PunchBox.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvPunchBox {
    NEW_NERVE(PunchBoxNrvWait, PunchBox, Wait);
    NEW_NERVE(PunchBoxNrvHit, PunchBox, Hit);
}  // namespace NrvPunchBox

void PunchBox_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)90.0f;
    (void)150.0f;
    (void)77.0f;
    (void)410.0f;
    (void)110.0f;
    (void)10.0f;
    (void)40.0f;
}

PunchBox::PunchBox(const char* pName) : LiveActor(pName) {
}

void PunchBox::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("PunchBox", nullptr, false);

    MR::connectToSceneMapObjStrongLight(this);

    initSound(2, false);
    initNerve(GET_NERVE(PunchBox, PunchBoxNrvWait));
    initEffectKeeper(3, nullptr, false);

    f32 radius = mScale.x * 90.0f;
    initHitSensor(1);
    TVec3f offset(0.0f, radius, 0.0f);
    MR::addHitSensorMapObj(this, "body", 8, radius, offset);

    MR::initCollisionParts(this, "PunchBox", getSensor("body"), nullptr);

    s32 arg0 = 1;
    s32 arg1 = 1;

    if (rIter.isValid()) {
        MR::getJMapInfoArg0NoInit(rIter, &arg0);
        MR::getJMapInfoArg1NoInit(rIter, &arg1);
    }

    mCoinCount = arg0;
    _94 = 1;
    _90 = 0;
    _92 = MR::getRandom(0l, 31l);
    _8C = 0;

    setNerve(GET_NERVE(PunchBox, PunchBoxNrvWait));

    MR::validateClipping(this);
    MR::setGroupClipping(this, rIter, 128);

    mHasShadow = false;
    if (arg1 != 0) {
        mHasShadow = true;
    }

    _98 = false;

    MR::declareCoin(this, mCoinCount);
    MR::declareStarPiece(this, 1);

    appear();

    if (mHasShadow) {
        MR::initShadowVolumeBox(this, TVec3f(150.0f, 150.0f, 150.0f));
        MR::setShadowVolumeStartDropOffset(this, nullptr, 77.0f);
        MR::setShadowVolumeEndDropOffset(this, nullptr, 150.0f);
        MR::onShadowVolumeCutDropLength(this, nullptr);
        MR::excludeCalcShadowToMyCollision(this, nullptr);
    }

    MR::addToAttributeGroupSearchTurtle(this);

    if (MR::useStageSwitchReadA(this, rIter)) {
        MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &PunchBox::kill));
    }

    getSensor("body")->invalidate();
}

void PunchBox::initAfterPlacement() {
    if (mHasShadow) {
        MR::onCalcGravity(this);
        MR::calcGravity(this);

        MR::onCalcShadowOneTime(this, nullptr);
    }
}

void PunchBox::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pReceiver)) {
        return;
    }

    if (pSender != getSensor("eye")) {
        return;
    }

    if (!MR::isSensorPlayer(pReceiver)) {
        return;
    }

    TVec3f distance(pReceiver->mPosition - pSender->mPosition);

    if (distance.length() < 410.0f) {
        _96 = true;
    }

    if (distance.length() < 110.0f) {
        _97 = true;
    }
}

void PunchBox::control() {
    if (_90 == 0) {
        _92++;
    }

    if (_8C != 0) {
        _8C--;
    }

    if (_98 != 0) {
        _98 = 0;
    }
}

void PunchBox::calcAndSetBaseMtx() {
    if (mHasShadow && (_92 & 0x7F) == 0x7F) {
        MR::onCalcShadowOneTime(this, nullptr);
    }

    LiveActor::calcAndSetBaseMtx();
}

void PunchBox::exeWait() {
    if (_90 != 0 && --_90 == 0) {
        _94 = 1;
    }

    if (_94 == 0) {
        setNerve(GET_NERVE(PunchBox, PunchBoxNrvHit));
    }

    _96 = false;
    _97 = false;
}

void PunchBox::exeHit() {
    if (getNerveStep() == 0) {
        getSensor("body")->invalidate();
    }

    if (getNerveStep() == 5) {
        MR::hideModel(this);
    }

    if (getNerveStep() == 15) {
        if (mCoinCount != 0) {
            MR::appearCoinPop(this, mPosition, mCoinCount);
        }

        makeActorDead();
    }
}

void PunchBox::kill() {
    if (MR::isDead(this)) {
        return;
    }

    MR::appearStarPiece(this, mPosition, 1, 10.0f, 40.0f, false);

    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");

    LiveActor::kill();
}

void PunchBox::doHit(HitSensor* pSender, HitSensor* pReceiver) {
    if (_94 != 0) {
        _94--;
    }

    if (_94 == 0) {
        MR::invalidateCollisionParts(this);
    }

    _90 = 30;

    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        MR::emitEffect(this, "BreakWater");
        MR::startSound(this, "SE_OJ_PUNCH_BOX_BREAK_W");
    } else {
        MR::emitEffect(this, "Break");
        MR::startSound(this, "SE_OJ_PUNCH_BOX_BREAK");
    }
}

bool PunchBox::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(PunchBox, PunchBoxNrvHit))) {
        return false;
    }

    if (MR::isMsgPlayerUpperPunch(msg)) {
        if (_94 != 0) {
            doHit(pSender, pReceiver);
            pSender->receiveMessage(ACTMES_REFLECT_V, pReceiver);
        }

        return true;
    }

    if (MR::isMsgPlayerHipDrop(msg) || MR::isMsgPlayerHipDropFloor(msg)) {
        if (_8C != 0) {
            doHit(pSender, pReceiver);
            return true;
        }

        return true;
    } else if (msg == ACTMES_JET_TURTLE_ATTACK) {
        doHit(pSender, pReceiver);
        return true;
    } else if (MR::isMsgPlayerHitAll(msg) && !MR::isOnPlayer(getSensor("body"))) {
        if (_94 != 0) {
            doHit(pSender, pReceiver);
        }

        return true;
    }

    return false;
}

bool PunchBox::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (_94 != 0) {
        doHit(pSender, pReceiver);
        return true;
    }

    return false;
}

bool PunchBox::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_IS_PUNCH_ENABLE) {
        if (_96 && _94 != 0) {
            return true;
        }

        return false;
    }

    if (msg == ACTMES_IS_BROKEN) {
        if (_96 && _94 != 0) {
            return false;
        }

        return true;
    }

    if (msg == ACTMES_IS_RUSH_ENABLE && _96 && _94 != 0) {
        _98 = true;
        return true;
    }

    if (msg == ACTMES_FLOOR_TOUCH) {
        _8C = 2;
    }

    if (msg == ACTMES_IS_OPD_SITE_IN && _94 != 0) {
        return true;
    }

    if ((msg == ACTMES_OPD_SHOOT || msg == ACTMES_BALL_DASH_WALL || msg == ACTMES_BALL_DASH_GROUND) && !MR::isSensorEye(pReceiver) && _94 != 0) {
        doHit(pSender, pReceiver);
        return true;
    }

    if (_94 != 0 && (msg == ACTMES_TURNIP_ATTACK || msg == ACTMES_PAKKUN_BALL_ATTACK)) {
        doHit(pSender, pReceiver);
        return true;
    }

    return false;
}
