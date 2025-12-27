#include "Game/NPC/Tico.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/NPC/TicoDemoGetPower.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/TalkUtil.hpp"

namespace NrvTico {
    NEW_NERVE(TicoNrvNoReaction, Tico, NoReaction);
    NEW_NERVE(TicoNrvReaction, Tico, Reaction);
    NEW_NERVE(TicoNrvDelight, Tico, Delight);
    NEW_NERVE(TicoNrvAppear, Tico, Appear);
    NEW_NERVE(TicoNrvWait, Tico, Wait);
    NEW_NERVE(TicoNrvTalk, Tico, Talk);
    NEW_NERVE(TicoNrvMeta, Tico, Meta);
    NEW_NERVE(TicoNrvBlue0, Tico, Blue0);
    NEW_NERVE(TicoNrvBlue1, Tico, Blue1);
    NEW_NERVE(TicoNrvRed0, Tico, Red0);
    NEW_NERVE(TicoNrvRed1, Tico, Red1);
    NEW_NERVE(TicoNrvRed2, Tico, Red2);
    NEW_NERVE(TicoNrvSpin0, Tico, Spin0);
    NEW_NERVE(TicoNrvGuide0, Tico, Guide0);
    NEW_NERVE(TicoNrvGuide1, Tico, Guide1);
    NEW_NERVE(TicoNrvGuide2, Tico, Guide2);
    NEW_NERVE(TicoNrvGuide3, Tico, Guide3);
    NEW_NERVE(TicoNrvLead0, Tico, Lead0);
};  // namespace NrvTico

namespace {
    static Color8 hPointLight[] = {Color8(0xFF, 0xFF, 0, 0xFF), Color8(0, 0x64, 0xFF, 0xFF), Color8(0, 0xFF, 0x32, 0xFF),
                                   Color8(0xFF, 0, 0, 0xFF),    Color8(0x78, 0, 0xFF, 0xFF), Color8(0xFF, 0x64, 0x64, 0xFF)};
};

Tico::Tico(const char* pName)
    : NPCActor(pName), _15C(0), _160(0.0f, 0.0f, 0.0f), _16C(0.0f), mDemoGetPower(nullptr), _174(0), _178(0), _17C(255, 255, 255, 255), _180(0),
      mDemoStarter(this) {}

void Tico::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    if (MR::isObjectName(rIter, "TicoBaby")) {
        pCollector->addArchive("TicoBaby");
        return;
    }

    pCollector->addArchive("Tico");
    pCollector->addArchive("TicoMiddle");
    pCollector->addArchive("TicoLow");
}

void Tico::initBase(s32 color) {
    JMapInfoIter iter(0, -1);
    initBase(iter, color);
}

void Tico::initBase(const JMapInfoIter& rIter, s32 color) {
    NPCActorCaps caps("Tico");
    caps.setDefault();
    caps._E = 0;
    caps.mUseShadow = true;
    caps.mWaitNerve = &NrvTico::TicoNrvWait::sInstance;
    caps._44 = "Body";
    caps.mSensorSize = 60.0f;
    caps.mSensorOffset.z = 0.0f;
    caps.mSensorOffset.y = 0.0f;
    caps.mSensorOffset.x = 0.0f;
    caps._6C = "Body";
    caps._38 = 0;

    if (color == -1) {
        caps.mObjectName = "TicoBaby";
    }

    setDefaults2();
    _13C = "Spin";
    _12C = 1000.0f;
    const char* wait = "Wait";
    const char* talk = "Talk";
    const char* fly = "Fly";
    mParam._14 = wait;
    mParam._18 = wait;
    mParam._1C = talk;
    mParam._20 = talk;
    _11C = fly;
    _120 = fly;
    _10C = 8.0f;
    _110 = 0.2f;
    mParam._C = 0.5f;
    mParam._10 = 12.0f;
    initialize(rIter, caps);
    if (color != -1) {
        MR::startBrk(this, "ColorChange");
        MR::setBrkFrameAndStop(this, color);
        _17C = hPointLight[color];
    } else {
        _17C = hPointLight[5];
    }

    _180 = &NrvTico::TicoNrvMeta::sInstance;
    _160.set< f32 >(mPosition);
    MR::startAction(this, "Wait");
    MR::setBckFrameAtRandom(this);
    _178 = MR::getJointMtx(this, "Body");
}

void Tico::initMessage(const JMapInfoIter& rIter, const char* pMsg) {
    if (initTalkCtrl(rIter, pMsg, TVec3f(0.0f, 120.0f, 0.0f), nullptr)) {
        TalkMessageCtrl* ctrl = mMsgCtrl;
        MR::registerKillFunc(mMsgCtrl, TalkMessageFunc< Tico >(this, &Tico::killFunc));
        MR::setDistanceToTalk(mMsgCtrl, 350.0f);
    }

    _174 = MR::createTalkCtrlDirect(this, rIter, "Common_Tico000", TVec3f(0.0f, 120.0f, 0.0f), nullptr);
    MR::offRootNodeAutomatic(_174);
}

void Tico::initMessage(const char* pMsg) {
    JMapInfoIter iter(0, -1);
    if (initTalkCtrl(iter, pMsg, TVec3f(0.0f, 120.0f, 0.0f), nullptr)) {
        TalkMessageCtrl* ctrl = mMsgCtrl;
        MR::registerKillFunc(mMsgCtrl, TalkMessageFunc< Tico >(this, &Tico::killFunc));
        MR::setDistanceToTalk(mMsgCtrl, 350.0f);
    }

    _174 = MR::createTalkCtrlDirect(this, iter, "Common_Tico000", TVec3f(0.0f, 120.0f, 0.0f), nullptr);
    MR::offRootNodeAutomatic(_174);
}

void Tico::init(const JMapInfoIter& rIter) {
    s32 color = 0;
    MR::getJMapInfoArg0NoInit(rIter, &color);
    s32 behavior = 0;
    s32 clr = color;
    MR::getJMapInfoArg1NoInit(rIter, &behavior);
    _15C = behavior;

    if (MR::isObjectName(rIter, "TicoBaby")) {
        clr = -1;
    }

    initBase(rIter, clr);
    initMessage(rIter, "Tico");
    MR::joinToGroupArray(this, rIter, nullptr, 32);
    AstroDemoFunction::tryRegisterDemoForTico(this, rIter);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        _15C = 0;
        if (MR::isRegisteredDemoActionAppear(this)) {
            makeActorDead();
        }

        if (MR::isDemoCast(this, "チコガイドデモ")) {
            if (clr == -1) {
                _15C = 5;
            } else {
                _15C = 6;
            }
        } else {
            if (MR::isDemoCast(this, "青チコ変身")) {
                _15C = 1;
            } else {
                if (MR::isDemoCast(this, "赤いスター")) {
                    _15C = 9;
                    MR::useStageSwitchWriteA(this, rIter);
                    MR::useStageSwitchWriteB(this, rIter);
                    DemoFunction::registerDemoTalkMessageCtrl(this, mMsgCtrl);
                }
            }
        }
    }

    switch (_15C) {
    case 5:
        mDemoGetPower = new TicoDemoGetPower(this, rIter);
        setNerve(&NrvTico::TicoNrvSpin0::sInstance);
        break;
    case 1:
        setNerve(&NrvTico::TicoNrvBlue0::sInstance);
        break;
    case 9:
        if (MR::isOnGameEventFlagRosettaTalkAboutTicoInTower()) {
            makeActorAppeared();
            setNerve(&NrvTico::TicoNrvRed1::sInstance);
        } else {
            setNerve(&NrvTico::TicoNrvRed0::sInstance);
        }
        break;
    case 6:
        MR::offRootNodeAutomatic(mMsgCtrl);
        setNerve(&NrvTico::TicoNrvGuide0::sInstance);
        break;
    case 7:
        setNerve(&NrvTico::TicoNrvLead0::sInstance);
        pushNerve(&NrvTico::TicoNrvAppear::sInstance);
        break;
    case 3:
        setNerve(&NrvTico::TicoNrvTalk::sInstance);
        break;
    case 8:
        setNerve(&NrvTico::TicoNrvNoReaction::sInstance);
        break;
    default:
        break;
    }
}

void Tico::calcAndSetBaseMtx() {
    MR::calcAndSetFloatBaseMtx(this, _16C);
}

void Tico::control() {
    if (_178) {
        TVec3f trans;
        MR::extractMtxTrans(_178, &trans);
        MR::requestPointLight(this, TVec3f(trans), _17C, 0.99864602f, -1);
    }

    if (isNerve(_180)) {
        NPCActor::control();
    } else {
        if (!MR::isTimeKeepDemoActive()) {
            MR::startLevelSound(this, "SE_SM_LV_TICO_WAIT", -1, -1, -1);
        }

        TVec3f v14(mPosition);
        JMathInlineVEC::PSVECSubtract(&v14, &_160, &v14);
        f32 v11 = PSVECMag(&v14);
        f32 v16 = (100.0f * MR::getLinerValueFromMinMax(v11, 1.0f, 11.0f, 0.2f, 1.0f));
        MR::startLevelSound(this, "SE_SM_LV_TICO_FLOAT", v16, -1, -1);
        _160.set< f32 >(mPosition);
        _16C = MR::calcFloatOffset(this, _16C, 150.0f);
        NPCActor::control();
    }
}

bool Tico::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return false;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        MR::limitedStarPieceHitSound();
        _E5 = true;
        return true;
    }

    return NPCActor::receiveMsgPlayerAttack(msg, pSender, pReceiver);
}

bool Tico::tryReaction() {
    if (_E5) {
        pushNerve(&NrvTico::TicoNrvDelight::sInstance);
        return true;
    }

    return MR::tryStartReactionAndPushNerve(this, &NrvTico::TicoNrvReaction::sInstance);
}

void Tico::setNerveMeta() {
    setNerve(_180);
}

void Tico::setNerveWait() {
    setNerve(mWaitNerve);
}

bool Tico::killFunc(u32) {
    setNerve(_180);
    return true;
}

void Tico::exeReaction() {
    if (_D8) {
        MR::startSound(this, "SE_SM_NPC_TRAMPLED", -1, -1);
        MR::startSound(this, "SE_SV_TICO_TRAMPLED", -1, -1);
    }

    if (isPointingSe()) {
        MR::startDPDHitSound();
        MR::startSound(this, "SE_SV_TICO_POINT", -1, -1);
    }

    if (_DB) {
        MR::limitedStarPieceHitSound();
        MR::startSound(this, "SE_BM_BUTLER_ABSORB", -1, -1);
    }

    MR::tryStartReactionAndPopNerve(this);
}

void Tico::exeDelight() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, _13C);
        MR::resetAndForwardNode(_174, MR::getRandom(0l, 5l));
    }

    MR::tryTalkForce(_174);
    if (MR::isActionEnd(this)) {
        popNerve();
    }
}

void Tico::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Appear");
    }

    if (MR::isBckStopped(this)) {
        popNerve();
    }
}

void Tico::exeNoReaction() {
    if (mMsgCtrl != nullptr) {
        if (MR::tryTalkNearPlayerAndStartTalkAction(this)) {
            setNerve(&NrvTico::TicoNrvWait::sInstance);
        }
    } else {
        MR::tryStartTurnAction(this);
    }
}

void Tico::exeWait() {
    if (!tryReaction()) {
        if (mMsgCtrl != nullptr) {
            MR::tryTalkNearPlayerAndStartTalkAction(this);
        } else {
            MR::tryStartTurnAction(this);
        }
    }
}

void Tico::exeMeta() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Metamorphosis");
    }

    if (MR::isBckStopped(this)) {
        MR::startSound(this, "SE_SM_METAMORPHOSE_SMOKE", -1, -1);
        kill();
    }
}

void Tico::exeBlue0() {
    if (!tryReaction()) {
        if (MR::tryTalkNearPlayerAtEndAndStartTalkAction(this)) {
            setNerve(&NrvTico::TicoNrvBlue1::sInstance);
            MR::startTimeKeepDemo(this, "青チコ変身", nullptr);
        }
    }
}

void Tico::exeBlue1() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Fly");
        MR::setRailCoordSpeed(this, (MR::getRailTotalLength(this) / MR::getDemoPartTotalStep("青チコ変身[移動]")));
    }

    MR::moveCoordAndFollowTrans(this);

    if (MR::isRailReachedGoal(this)) {
        setNerve(&NrvTico::TicoNrvMeta::sInstance);
    }
}

void Tico::exeRed1() {
    if (!tryReaction()) {
        MR::tryStartTurnAction(this);
        if (MR::isNearPlayer(this, 400.0f)) {
            setNerve(&NrvTico::TicoNrvRed2::sInstance);
        }
    }
}

void Tico::exeRed2() {
    if (MR::isFirstStep(this)) {
        mDemoStarter.start();
    }

    if (mDemoStarter.update()) {
        MR::tryStartTimeKeepDemoMarioPuppetable(this, "赤いスター", "赤いスター[開始]");
        setNerve(&NrvTico::TicoNrvWait::sInstance);
    }
}

void Tico::exeGuide0() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Appear");
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvTico::TicoNrvGuide1::sInstance);
    }
}

void Tico::exeGuide1() {
    if (MR::countShowGroupMember(this) == 0) {
        MR::forwardNode(mMsgCtrl);
        MR::forwardNode(mMsgCtrl);
        setNerve(&NrvTico::TicoNrvGuide2::sInstance);
    } else {
        if (MR::countHideGroupMember(this) != 0) {
            MR::forwardNode(mMsgCtrl);
            setNerve(&NrvTico::TicoNrvGuide2::sInstance);
        } else {
            MR::tryTalkNearPlayerAndStartTalkAction(this);
            if (tryReaction()) {
                return;
            }
        }
    }
}

void Tico::exeGuide2() {
    if (MR::countShowGroupMember(this) == 0) {
        MR::forwardNode(mMsgCtrl);
        setNerve(&NrvTico::TicoNrvGuide3::sInstance);
    } else {
        MR::tryTalkNearPlayerAndStartTalkAction(this);
        if (tryReaction()) {
            return;
        }
    }
}

void Tico::exeGuide3() {
    if (!MR::isFirstStep(this)) {
        if (MR::tryTalkNearPlayerAtEndAndStartTalkAction(this) && MR::isExistNextNode(mMsgCtrl)) {
            MR::forwardNode(mMsgCtrl);
        } else {
            if (tryReaction()) {
                return;
            }
        }
    }
}

void Tico::exeLead0() {
    if (mMsgCtrl != nullptr) {
        MR::tryTalkForceAndStartMoveTalkAction(this);
    } else {
        MR::tryStartMoveTurnAction(this);
    }

    if (!tryReaction() && !MR::isRailGoingToEnd(this)) {
        setNerve(mWaitNerve);
    }
}

void Tico::exeSpin0() {
    mDemoGetPower->updateNerve();
}

void Tico::exeRed0() {
    if (MR::tryStartTimeKeepDemoMarioPuppetable(this, "赤いスター", "赤いスター[開始]")) {
        setNerve(&NrvTico::TicoNrvWait::sInstance);
    }
}

void Tico::exeTalk() {
    if (!tryReaction()) {
        if (MR::tryTalkNearPlayerAtEndAndStartTalkAction(this)) {
            setNerve(&NrvTico::TicoNrvMeta::sInstance);
        }
    }
}
