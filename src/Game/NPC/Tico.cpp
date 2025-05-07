#include "Game/NPC/Tico.hpp"
#include "Game/NPC/TicoDemoGetPower.hpp"
#include "Game/Util/NPCUtil.hpp"

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

void Tico::initBase(s32 a1) {
    JMapInfoIter iter(0, -1);
    initBase(iter, a1);
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
