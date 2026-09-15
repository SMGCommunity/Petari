#include "Game/NPC/Rabbit.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

namespace NrvRabbit {
    NEW_NERVE(RabbitNrvAppear, Rabbit, Appear);
    NEW_NERVE(RabbitNrvAppearLand, Rabbit, AppearLand);
    NEW_NERVE(RabbitNrvWait, Rabbit, Wait);
    NEW_NERVE(RabbitNrvForwardLand, Rabbit, ForwardLand);
    NEW_NERVE(RabbitNrvPreJump, Rabbit, PreJump);
    NEW_NERVE(RabbitNrvMove, Rabbit, Move);
    NEW_NERVE(RabbitNrvGoal, Rabbit, Goal);
    NEW_NERVE(RabbitNrvFinish, Rabbit, Finish);
    NEW_NERVE(RabbitNrvReaction, Rabbit, Reaction);
    NEW_NERVE(RabbitNrvTalk, Rabbit, Talk);
    NEW_NERVE(RabbitNrvJumpV, Rabbit, JumpV);
    NEW_NERVE(RabbitNrvJumpH, Rabbit, JumpH);
    NEW_NERVE(RabbitNrvBackwardLand, Rabbit, BackwardLand);
    NEW_NERVE(RabbitNrvPreJumpBack, Rabbit, PreJumpBack);
    NEW_NERVE(RabbitNrvNear, Rabbit, Near);
};  // namespace NrvRabbit

Rabbit::Rabbit(const char* pName) : NPCActor(pName) {
}

Rabbit::~Rabbit() {
}

void Rabbit::init(const JMapInfoIter& rIter) {
    const char* name;
    MR::getObjectName(&name, rIter);
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("MoonRabbit", nullptr, false);
    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);
    initSound(4, false);
    initBinder(10.0f, 10.0f, 0);
    MR::onCalcGravity(this);
    initHitSensor(1);
    MR::addHitSensorMtxNpc(this, "body", 8, 60.0f, MR::getJointMtx(this, "Spine"), TVec3f(0.0f, 0.0f, 0.0f));
    MR::initStarPointerTargetAtJoint(this, "Spine", 60.0f, TVec3f(0.0f, 60.0f, 0.0f));
    initEffectKeeper(0, nullptr, false);
    MR::initShadowFromCSV(this, "Shadow");
    mLodCtrl = MR::createLodCtrlNPC(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    s32 arg;
    MR::getJMapInfoArg0NoInit(rIter, &arg);
    MR::startBtk(this, "MoonRabbit");
    MR::setBrkFrameAndStop(this, arg);
    if (MR::getJMapInfoMessageID(rIter, &arg)) {
        TalkMessageCtrl* talkCtrl = MR::createTalkCtrl(this, rIter, name, TVec3f(0.0f, 160.0f, 0.0f), nullptr);
        mTalkCtrl = talkCtrl;
        MR::onRootNodeAutomatic(talkCtrl);
        MR::useStageSwitchReadA(this, rIter);
        MR::useStageSwitchReadB(this, rIter);
    }

    s32 arg1;
    if (MR::tryRegisterDemoCast(this, rIter)) {
        if (MR::getDemoCastID(rIter)) {
            _15C = 3;
        } else {
            _15C = 2;
        }
    } else {
        MR::getJMapInfoArg1NoInit(rIter, &arg1);
        if (arg1 == 2) {
            _15C = 4;
            initNerve(&NrvRabbit::RabbitNrvTalk::sInstance);
        } else if (arg1 > 2) {
            if (arg1 < 4) {
                _15C = 5;
                initNerve(&NrvRabbit::RabbitNrvTalk::sInstance);
            }
        } else if (arg1) {
            if (arg1 >= 0) {
                _15C = 1;
                initNerve(&NrvRabbit::RabbitNrvTalk::sInstance);
            }
        } else {
        }
    }
}

void Rabbit::control() {
}

void Rabbit::exeAppear() {
}

void Rabbit::exeAppearLand() {
}

void Rabbit::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "AppearLand", nullptr);
    }

    if (MR::isBckStopped(this)) {
        popNerve();
    }
}

void Rabbit::exeGoal() {
}

void Rabbit::exeFinish() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Change", nullptr);
        MR::startSound(this, "SE_SM_RABBIT_CHANGE_JUMP");
    }

    if (MR::isBckStopped(this)) {
        MR::startSound(this, "SE_SM_RABBIT_CHANGE_EFFECT");
        if (MR::isValidSwitchDead(this)) {
            MR::onSwitchDead(this);
        }
        kill();
    }
}

void Rabbit::calcAndSetBaseMtx() {
    TVec3f pos(mPosition);
    mPosition += mGravity * _168;
    NPCActor::calcAndSetBaseMtx();
    mPosition = pos;
}

void Rabbit::calcRailPos(TVec3f* v1) {
}

bool Rabbit::isNeedTurn(const TVec3f& a1) {
    TVec3f a;
    _A0.getXDir(a);
    return MR::diffAngleAbs(a1, a) > 0.78539819f;
}

void Rabbit::updateJump() {
    if (isNerve(&NrvRabbit::RabbitNrvAppear::sInstance)) {
        if (_162) {
            _168 = 0.0f;
        }
        if (_168 < 0.0f) {
            _16C += 3.0f;
            if (_16C >= 0.0f) {
                _160 = true;
            }
        } else {
            if (isNerve(&NrvRabbit::RabbitNrvMove::sInstance) || isNerve(&NrvRabbit::RabbitNrvNear::sInstance)) {
                _16C = -20.0f * _180;
            } else {
                _16C = 0.0f;
            }
            _168 = 0.0f;
            if (_160) {
                _162 = true;
                _160 = false;
            }
        }
        _168 += _16C;
        if (_168 >= 0.0f) {
            _168 = 0.0f;
        }
    }
}

void Rabbit::exeForwardLand() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "JumpEnd", nullptr);
    }

    if (MR::isBckStopped(this)) {
        if (MR::isNearPlayer(mTalkCtrl, 500.0f)) {
            setNerve(&NrvRabbit::RabbitNrvPreJump::sInstance);
        } else {
            _164 = 120;
            setNerve(&NrvRabbit::RabbitNrvWait::sInstance);
        }
    }
}

void Rabbit::exePreJump() {
    if (MR::isFirstStep(this)) {
        MR::setRailDirectionToEnd(this);
        MR::startBck(this, "JumpStart", nullptr);
        MR::startSound(this, "SE_SM_RABBIT_HOP");
    }

    MR::blendQuatUpFront(&_A0, -mGravity, MR::getRailDirection(this), 0.5f, 0.5f);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvRabbit::RabbitNrvMove::sInstance);
    }
}

void Rabbit::exeMove() {
}

void Rabbit::exeBackwardLand() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "JumpEnd", nullptr);
    }

    if (MR::isBckStopped(this)) {
        if (MR::isNearPlayer(mTalkCtrl, 500.0f)) {
            setNerve(&NrvRabbit::RabbitNrvWait::sInstance);
        } else {
            setNerve(&NrvRabbit::RabbitNrvPreJumpBack::sInstance);
        }
    }
}

void Rabbit::exePreJumpBack() {
    if (MR::isFirstStep(this)) {
        MR::setRailDirectionToStart(this);
        MR::startBck(this, "JumpStart", nullptr);
        MR::startSound(this, "SE_SM_RABBIT_HOP");
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvRabbit::RabbitNrvNear::sInstance);
    }
}

void Rabbit::exeNear() {
    if (MR::isFirstStep(this)) {
        MR::setRailDirectionToStart(this);
        _162 = 0;
        MR::startBck(this, "Jump", nullptr);
        MR::startSound(this, "SE_SM_RABBIT_JUMP");
        _170 = 18.0f;
        MR::moveCoordToNearestPos(this, mPosition);
        MR::moveCoord(this, _170);
        TVec3f railPos(MR::getRailPos(this));
        calcRailPos(&railPos);
        _174 = -railPos;
        MR::normalizeOrZero(&_174);
    }
    mPosition += _174 * _170;
    _170 *= 0.98f;
    if (MR::isNearPlayer(mTalkCtrl, 500.0f)) {
        setNerve(&NrvRabbit::RabbitNrvWait::sInstance);
    } else if (MR::isRailReachedGoal(this) && -mPosition.length() < 10.0f) {
        setNerve(&NrvRabbit::RabbitNrvWait::sInstance);
    } else {
        MR::calcNearestRailCoord(this, *MR::getPlayerPos());
        if (_162) {
            setNerve(&NrvRabbit::RabbitNrvBackwardLand::sInstance);
        }
    }
}

void Rabbit::exeReaction() {
    if (_E4) {
        MR::startSound(this, "SE_SM_NPC_TRAMPLED");
        MR::startSound(this, "SE_SV_RABBIT_TRAMPLED");
    }

    if (isPointingSe()) {
        MR::startDPDHitSound();
    }

    if (_DB) {
        MR::startSound(this, "SE_SM_LV_RABBIT_POINT");
    }

    if (_E4) {
        MR::startSound(this, "SE_SV_RABBIT_SPIN");
        MR::startSound(this, "SE_SM_RABBIT_SPIN");
    }

    if (_E4) {
        MR::limitedStarPieceHitSound();
        MR::startSound(this, "SE_SM_RABBIT_STAR_PIECE_HIT");
        MR::startSound(this, "SE_SV_RABBIT_STAR_PIECE_HIT");
    }

    MR::tryStartReactionAndPopNerve(this);
}

void Rabbit::exeTalk() {
    if (MR::isFirstStep(this)) {
    }

    if (!MR::tryStartReactionAndPushNerve(this, &NrvRabbit::RabbitNrvReaction::sInstance)) {
        if (!MR::tryTalkNearPlayerAndStartTalkAction(this) || MR::isShortTalk(mTalkCtrl)) {
            if (_15C == 4 && MR::isGreaterStep(this, 180)) {
                pushNerve(&NrvRabbit::RabbitNrvJumpV::sInstance);
            } else if (_15C == 5) {
                if (MR::isGreaterStep(this, 180)) {
                    pushNerve(&NrvRabbit::RabbitNrvJumpH::sInstance);
                }
            }
        }
    }
}

void Rabbit::exeJumpV() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "SpinLecture");
    }

    if (MR::isBckStopped(this)) {
        popNerve();
    }
}

void Rabbit::exeJumpH() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "JumpSpinLectureMove");
    }

    if (MR::isBckLooped(this)) {
        popNerve();
    }
}
