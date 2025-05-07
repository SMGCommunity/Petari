#include "Game/NPC/Tico.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/NPC/TicoDemoGetPower.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/NPCUtil.hpp"
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
};

namespace {
    static Color8 hPointLight[6] = {
        Color8(0xFF, 0xFF, 0, 0xFF),
        Color8(0, 0x64, 0xFF, 0xFF),
        Color8(0, 0xFF, 0x32, 0xFF),
        Color8(0xFF, 0, 0, 0xFF),
        Color8(0x78, 0, 0xFF, 0xFF),
        Color8(0xFF, 0x64,0x64, 0xFF)
    };
};

Tico::Tico(const char *pName) : NPCActor(pName), _15C(0), _160(0.0f, 0.0f, 0.0f), 
    _16C(0.0f), mDemoGetPower(nullptr), _174(0), _178(0), _17C(255, 255, 255, 255), 
    _180(0), mDemoStarter(this) {

}

void Tico::makeArchiveList(NameObjArchiveListCollector *pCollector, const JMapInfoIter &rIter) {
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

void Tico::initBase(const JMapInfoIter &rIter, s32 color) {
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

    _130 = "Spin";
    _134 = "Trampled";
    _138 = "Pointing";
    _13C = "Spin";
    _12C = 1000.0f;
    _FC = "Wait";
    _100 = "Wait";
    _104 = "Talk";
    _108 = "Talk";
    _11C = "Fly";
    _120 = "Fly";
    _10C = 8.0f;
    _110 = 0.2f;
    _F4 = 0.5f;
    _F8 = 12.0f;
    initialize(rIter, caps);
    if (color != -1) {
        MR::startBrk(this, "ColorChange");
        MR::setBrkFrameAndStop(this, color);
        _17C = hPointLight[color];   
    }
    else {
        _17C = hPointLight[5];
    }

    _180 = &NrvTico::TicoNrvMeta::sInstance;
    _160.set<f32>(mPosition);
    MR::startAction(this, "Wait");
    MR::setBckFrameAtRandom(this);
    _178 = MR::getJointMtx(this, "Body");
}

void Tico::initMessage(const JMapInfoIter &rIter, const char *pMsg) {
    if (initTalkCtrl(rIter, pMsg, TVec3f(0.0f, 120.0f, 0.0f), nullptr)) {
        TalkMessageCtrl* ctrl = mMsgCtrl;
        MR::registerKillFunc(mMsgCtrl, TalkMessageFunc<Tico>(this, &Tico::killFunc));
        MR::setDistanceToTalk(mMsgCtrl, 350.0f);
    }

    _174 = MR::createTalkCtrlDirect(this, rIter, "Common_Tico000", TVec3f(0.0f, 120.0f, 0.0f), nullptr);
    MR::offRootNodeAutomatic(_174);
}

void Tico::initMessage(const char *pMsg) {
    JMapInfoIter iter(0, -1);
    if (initTalkCtrl(iter, pMsg, TVec3f(0.0f, 120.0f, 0.0f), nullptr)) {
        TalkMessageCtrl* ctrl = mMsgCtrl;
        MR::registerKillFunc(mMsgCtrl, TalkMessageFunc<Tico>(this, &Tico::killFunc));
        MR::setDistanceToTalk(mMsgCtrl, 350.0f);
    }

    _174 = MR::createTalkCtrlDirect(this, iter, "Common_Tico000", TVec3f(0.0f, 120.0f, 0.0f), nullptr);
    MR::offRootNodeAutomatic(_174);
}

void Tico::init(const JMapInfoIter &rIter) {
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
            }
            else {
                _15C = 6;
            }
        }
        else {
            if (MR::isDemoCast(this, "青チコ変身")) {
                _15C = 1;
            }
            else {
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
            }
            else {
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
    }
    else {
        if (!MR::isTimeKeepDemoActive()) {
            MR::startLevelSound(this, "SE_SM_LV_TICO_WAIT", -1, -1, -1);
        }

        TVec3f v14(mPosition);
        JMathInlineVEC::PSVECSubtract(&v14, &_160, &v14);
        f32 v11 = PSVECMag(&v14);
        f32 v16 = (100.0f * MR::getLinerValueFromMinMax(v11, 1.0f, 11.0f, 0.2f, 1.0f));
        MR::startLevelSound(this, "SE_SM_LV_TICO_FLOAT", v16, -1, -1);
        _160.set<f32>(mPosition);
        _16C = MR::calcFloatOffset(this, _16C, 150.0f);
        NPCActor::control();
    }
}

bool Tico::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return false;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        MR::limitedStarPieceHitSound();
        _E5 = true;
        return true;
    }

    return NPCActor::receiveMsgPlayerAttack(msg, a2, a3);
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
