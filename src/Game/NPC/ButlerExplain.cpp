#include "Game/NPC/ButlerExplain.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "revolution/types.h"

namespace NrvButlerExplain {
    NEW_NERVE(ButlerExplainNrvDemo, ButlerExplain, Demo);
};

ButlerExplain::ButlerExplain(const char* pName) : NPCActor(pName) {}

ButlerExplain::~ButlerExplain() {}

void ButlerExplain::init(const JMapInfoIter& rIter) {
    NPCActorCaps caps("ButlerExplain");
    caps.setDefault();
    caps.mObjectName = "Butler";
    NPCActor::initialize(rIter, caps);
    if (MR::tryRegisterDemoCast(this, rIter)) {
        DemoFunction::tryCreateDemoTalkAnimCtrlForActor(this, "DemoWithButler", nullptr);
        MR::registerDemoActionFunctor(this, MR::Functor_Inline(this, &ButlerExplain::startDemo), "DemoWithButler");
        DemoFunction::registerDemoTalkMessageCtrl(this, mTalk);
    }
    MR::tryRegisterDemoCast(this, "グランドスター１帰還", rIter);
}

void ButlerExplain::control() {
    if (_D8) {
        MR::startSound(this, "SE_SM_NPC_TRAMPLED", -1, -1);
        MR::startSound(this, "SE_SV_BUTLER_TRAMPLED", -1, -1);
    }
    NPCActor::control();
}

void ButlerExplain::startDemo() {
    setNerve(&NrvButlerExplain::ButlerExplainNrvDemo::sInstance);
}

void ButlerExplain::exeDemo() {}
