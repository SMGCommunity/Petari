#include "Game/NPC/Peach.hpp"

namespace NrvPeach {
    NEW_NERVE(PeachNrvWait, Peach, Wait);
};

Peach::Peach(const char* pName)
    : NPCActor(pName) {
}

void Peach::init(const JMapInfoIter& rIter) {
    NPCActorCaps caps("Peach");
    caps.setDefault();
    caps.mWaitNerve = &NrvPeach::PeachNrvWait::sInstance;
    NPCActor::initialize(rIter, caps);
}

void Peach::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Help", nullptr);
    }

    if (mMsgCtrl != nullptr) {
        MR::tryTalkNearPlayer(mMsgCtrl);
    }
}

Peach::~Peach() {
}
