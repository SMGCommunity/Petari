#include "Game/NPC/Rabbit.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ParabolicPath.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

namespace {
    static f32 cJumpSpeed = 20.0f;
    static f32 cProgressSpeed = 18.0f;
    static f32 cGravity = 3.0f;
    static f32 cFinalDisappearRadius = 300.0f;
    static f32 cDistEscape = 400.0f;
    static f32 cDistWait = 500.0f;
    static f32 cDistNear = 600.0f;
    static f32 cAppearHeight = 150.0f;
    static f32 cAppearHop = 10.0f;
    static f32 cRegistNormal = 0.98f;
};  // namespace

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
    MR::initDefaultPose(this, rIter);
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
    s32 colorFrameArg = 0;
    MR::getJMapInfoArg0NoInit(rIter, &colorFrameArg);
    MR::startBtk(this, "MoonRabbit");
    MR::setBtkFrameAndStop(this, colorFrameArg);

    s32 messageId;
    if (MR::getJMapInfoMessageID(rIter, &messageId)) {
        mMsgCtrl = MR::createTalkCtrl(this, rIter, name, TVec3f(0.0f, 160.0f, 0.0f), nullptr);
        MR::onRootNodeAutomatic(mMsgCtrl);
        MR::useStageSwitchReadA(this, rIter);
        MR::useStageSwitchReadB(this, rIter);
    }

    s32 behaviorArg = 0;
    if (MR::tryRegisterDemoCast(this, rIter)) {
        if (MR::getDemoCastID(rIter) == 0) {
            mBehavior = Behavior_Demo1;
        } else {
            mBehavior = Behavior_Demo2;
        }
    } else {
        MR::getJMapInfoArg1NoInit(rIter, &behaviorArg);
        switch (behaviorArg) {
        case 0:
            if (MR::checkJMapDataEntries(rIter)) {
                initRailRider(rIter);
                MR::moveCoordAndTransToNearestRailPos(this);
            }

            mBehavior = Behavior_Wait;
            initNerve(GET_NERVE(Rabbit, RabbitNrvWait));
            break;

        case 1:
            mBehavior = Behavior_Unknown;
            initNerve(GET_NERVE(Rabbit, RabbitNrvTalk));
            break;

        case 2:
            mBehavior = Behavior_UpJump;
            initNerve(GET_NERVE(Rabbit, RabbitNrvTalk));
            break;
        case 3:
            mBehavior = Behavior_LongJump;
            initNerve(GET_NERVE(Rabbit, RabbitNrvTalk));
            break;
        }
    }

    _168 = 0.0f;
    _16C = 0.0f;
    _180 = 1.0f;
    _162 = 1;
    _160 = false;
    _164 = 0;

    switch (mBehavior) {
    case Behavior_Demo1:
        makeActorDead();
        break;

    case Behavior_Demo2:
        makeActorDead();
        break;

    default:
        if (MR::useStageSwitchReadAppear(this, rIter)) {
            MR::syncStageSwitchAppear(this);
            makeActorDead();
            pushNerve(GET_NERVE(Rabbit, RabbitNrvAppear));
        } else {
            makeActorAppeared();
            MR::startBck(this, "Wait2");
            MR::emitEffect(this, "Light");
        }

        break;
    }

    MR::setClippingFar100m(this);
    const char* const turn = "TurnSmall";
    const char* const wait = "Wait";
    const char* const reaction = "Reaction";
    const char* const pointing = "Pointing";
    const char* const press = "Press";
    const char* const spin = "Spin";
    mParam._14 = wait;
    mParam._18 = turn;
    const char* talk = "Talk";
    mParam._1C = talk;
    mParam._20 = turn;
    setDefaults(reaction, pointing, press, spin);
    _12C = 450.0f;

    if (mBehavior == Behavior_LongJump) {
        mParam._0 = false;
        mParam._1 = false;
    }
}

void Rabbit::control() {
    if (_D8) {
        MR::startSound(this, "SE_SM_NPC_TRAMPLED");
        MR::startSound(this, "SE_SV_RABBIT_TRAMPLED");
    }

    NPCActor::control();
    updateJump();

    if (MR::calcDistanceToPlayer(this) > 5000.0f) {
        MR::validateClipping(this);
    } else if (MR::calcDistanceToPlayer(this) < 3000.0f) {
        MR::invalidateClipping(this);
    }
}

void Rabbit::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Appear");
    }

    ParabolicPath path = ParabolicPath();
    TVec3f v9, v8;
    v8.set(0.0f, ::cAppearHeight, 0.0f);
    path.initFromUpVector(v8, TVec3f(0.0f, 0.0f, 0.0f), -mGravity, ::cAppearHop);
    path.calcPosition(&v9, getNerveStep() / MR::getBckFrameMax(this));
    _168 = -v9.y;

    if (MR::isBckStopped(this)) {
        _168 = 0.0f;
        setNerve(GET_NERVE(Rabbit, RabbitNrvAppearLand));
    }
}

void Rabbit::exeAppearLand() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "AppearLand");
    }

    if (MR::isBckStopped(this)) {
        popNerve();
    }
}

void Rabbit::exeWait() {
    if (MR::isBckOneTimeAndStopped(this) && _162) {
        MR::startBck(this, "Wait2");
    }

    if (getNerveStep() > 30) {
        TVec3f playerPos = *MR::getPlayerPos() - mPosition;
        MR::normalizeOrZero(&playerPos);
        if (!MR::isNearZero(playerPos)) {
            if (isNeedTurn(playerPos) && MR::isBckOneTimeAndStopped(this)) {
                MR::startBck(this, "Turn");
            }

            MR::blendQuatUpFront(&_A0, -mGravity, playerPos, 0.5f, 0.5f);
        }

        if (MR::isBckOneTimeAndStopped(this)) {
            MR::startBck(this, "Wait2");
        }
    }

    if (MR::isNearPlayer(this, ::cDistEscape)) {
        setNerve(GET_NERVE(Rabbit, RabbitNrvPreJump));
    } else if (_164) {
        _164--;
    } else if (!MR::isNearPlayer(this, ::cDistNear)) {
        const f32 nearestCoord = MR::calcNearestRailCoord(this, *MR::getPlayerPos());
        if (nearestCoord < MR::getRailCoord(this) && MR::getRailCoord(this) > 3.0f * ::cProgressSpeed) {
            setNerve(GET_NERVE(Rabbit, RabbitNrvNear));
        }
    }
}

void Rabbit::exeGoal() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
    }

    if (getNerveStep() > 30) {
        TVec3f playerPos = *MR::getPlayerPos() - mPosition;
        MR::normalizeOrZero(&playerPos);
        if (!MR::isNearZero(playerPos)) {
            MR::blendQuatUpFront(&_A0, -mGravity, playerPos, 0.5f, 0.5f);
        }
    }

    if (MR::isNearPlayer(this, ::cFinalDisappearRadius)) {
        setNerve(GET_NERVE(Rabbit, RabbitNrvFinish));
    }
}

void Rabbit::exeFinish() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Change");
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

void Rabbit::calcRailPos(TVec3f* pPos) {
    Triangle triangle = Triangle();
    TVec3f pos;
    if (MR::getFirstPolyOnLineToMap(&pos, &triangle, *pPos + mGravity * -500.0f, mGravity * 1000.0f)) {
        *pPos = pos;
    }

    _180 = MR::sqrt(MR::max(1.0f, MR::abs((*pPos - mPosition).y / 80.0f)));
}

bool Rabbit::isNeedTurn(const TVec3f& rA1) {
    TVec3f v2;
    _A0.getZDir(v2);
    return MR::diffAngleAbs(rA1, v2) > 0.78539819f;
}

void Rabbit::updateJump() {
    if (!isNerve(GET_NERVE(Rabbit, RabbitNrvAppear))) {
        if (_162) {
            _168 = 0.0f;
            return;
        }

        if (_168 >= 0.0f) {
            if (isNerve(GET_NERVE(Rabbit, RabbitNrvMove)) || isNerve(GET_NERVE(Rabbit, RabbitNrvNear))) {
                _16C = -::cJumpSpeed * _180;
            } else {
                _16C = 0.0f;
            }

            _168 = 0.0f;

            if (_160) {
                _162 = 1;
                _160 = false;
                return;
            }
        } else {
            _16C += ::cGravity;

            if (_16C >= 0.0f) {
                _160 = true;
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
        MR::startBck(this, "JumpEnd");
    }

    if (MR::isBckStopped(this)) {
        if (!MR::isNearPlayer(this, ::cDistWait)) {
            _164 = 120;
            setNerve(GET_NERVE(Rabbit, RabbitNrvWait));
        } else {
            setNerve(GET_NERVE(Rabbit, RabbitNrvPreJump));
        }
    }
}

void Rabbit::exePreJump() {
    if (MR::isFirstStep(this)) {
        MR::setRailDirectionToEnd(this);
        MR::startBck(this, "JumpStart");
        MR::startSound(this, "SE_SM_RABBIT_HOP");
    }

    TVec3f railDir(MR::getRailDirection(this));
    MR::blendQuatUpFront(&_A0, -mGravity, railDir, 0.5f, 0.5f);

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Rabbit, RabbitNrvMove));
    }
}

void Rabbit::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::setRailDirectionToEnd(this);
        MR::startBck(this, "Jump");
        MR::startSound(this, "SE_SM_RABBIT_JUMP");
        _162 = 0;
        _170 = ::cProgressSpeed;
        f32 distance = MR::getPlayerPos()->distance(mPosition);
        f32 hopStrengh = 1.0f;

        if (distance < 100.0f) {
            hopStrengh = 2.0f;
        } else if (distance < 150.0f) {
            hopStrengh = 1.8f;
        } else if (distance < 200.0f) {
            hopStrengh = 1.6f;
        } else if (distance < 250.0f) {
            hopStrengh = 1.4f;
        } else if (distance < 300.0f) {
            hopStrengh = 1.2f;
        } else if (distance < 350.0f) {
            hopStrengh = 1.1f;
        }

        _170 *= hopStrengh;
        MR::moveCoordToNearestPos(this, mPosition);
        MR::moveCoord(this, 20.0f * _170);
        TVec3f railPos(MR::getRailPos(this));
        calcRailPos(&railPos);
        _174 = railPos - mPosition;
        MR::normalizeOrZero(&_174);
    }

    mPosition += _174 * _170;
    _170 *= ::cRegistNormal;

    if (MR::isRailReachedGoal(this) && (mPosition - MR::getRailPos(this)).length() < 10.0f) {
        setNerve(GET_NERVE(Rabbit, RabbitNrvGoal));
    } else if (_162) {
        setNerve(GET_NERVE(Rabbit, RabbitNrvForwardLand));
    }
}

void Rabbit::exeBackwardLand() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "JumpEnd");
    }

    if (MR::isBckStopped(this)) {
        if (MR::isNearPlayer(this, ::cDistWait)) {
            setNerve(GET_NERVE(Rabbit, RabbitNrvWait));
        } else {
            setNerve(GET_NERVE(Rabbit, RabbitNrvPreJumpBack));
        }
    }
}

void Rabbit::exePreJumpBack() {
    if (MR::isFirstStep(this)) {
        MR::setRailDirectionToStart(this);
        MR::startBck(this, "JumpStart");
        MR::startSound(this, "SE_SM_RABBIT_HOP");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Rabbit, RabbitNrvNear));
    }
}

void Rabbit::exeNear() {
    if (MR::isFirstStep(this)) {
        MR::setRailDirectionToStart(this);
        _162 = 0;
        MR::startBck(this, "Jump");
        MR::startSound(this, "SE_SM_RABBIT_JUMP");
        _170 = ::cProgressSpeed;
        MR::moveCoordToNearestPos(this, mPosition);
        MR::moveCoord(this, 20.0f * _170);
        TVec3f railPos(MR::getRailPos(this));
        calcRailPos(&railPos);
        _174 = railPos - mPosition;
        MR::normalizeOrZero(&_174);
    }

    mPosition += _174 * _170;
    _170 *= ::cRegistNormal;

    if (MR::isNearPlayer(this, ::cDistWait)) {
        setNerve(GET_NERVE(Rabbit, RabbitNrvWait));
    } else if (MR::isRailReachedGoal(this) && (mPosition - MR::getRailPos(this)).length() < 10.0f) {
        setNerve(GET_NERVE(Rabbit, RabbitNrvWait));
    } else {
        MR::calcNearestRailCoord(this, *MR::getPlayerPos());
        if (_162) {
            setNerve(GET_NERVE(Rabbit, RabbitNrvBackwardLand));
        }
    }
}

void Rabbit::exeReaction() {
    if (_D8) {
        MR::startSound(this, "SE_SM_NPC_TRAMPLED");
        MR::startSound(this, "SE_SV_RABBIT_TRAMPLED");
    }

    if (isPointingSe()) {
        MR::startDPDHitSound();
    }

    if (_E4) {
        MR::startLevelSound(this, "SE_SM_LV_RABBIT_POINT");
    }

    if (_D9) {
        MR::startSound(this, "SE_SV_RABBIT_SPIN");
        MR::startSound(this, "SE_SM_RABBIT_SPIN");
    }

    if (_DB) {
        MR::limitedStarPieceHitSound();
        MR::startSound(this, "SE_SM_RABBIT_STAR_PIECE_HIT");
        MR::startSound(this, "SE_SV_RABBIT_STAR_PIECE_HIT");
    }

    if (MR::tryStartReactionAndPopNerve(this)) {
        return;
    }
}

void Rabbit::exeTalk() {
    if (MR::isFirstStep(this)) {
    }

    if (!MR::tryStartReactionAndPushNerve(this, GET_NERVE(Rabbit, RabbitNrvReaction)) &&
        (!MR::tryTalkNearPlayerAndStartTalkAction(this) || MR::isShortTalk(mMsgCtrl))) {
        if (mBehavior == Behavior_UpJump && MR::isGreaterStep(this, 180)) {
            pushNerve(GET_NERVE(Rabbit, RabbitNrvJumpV));
        } else if (mBehavior == Behavior_LongJump) {
            if (MR::isGreaterStep(this, 180)) {
                pushNerve(GET_NERVE(Rabbit, RabbitNrvJumpH));
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
