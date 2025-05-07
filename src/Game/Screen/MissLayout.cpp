#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/MissLayout.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    NEW_NERVE(MissLayoutNrvWait, MissLayout, Wait);
    NEW_NERVE(MissLayoutNrvActive, MissLayout, Active);
    NEW_NERVE(MissLayoutNrvEnd, MissLayout, End);
};

MissLayout::MissLayout() :
    LayoutActor("ミス!", true)
{}

void MissLayout::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayout(this);
    initLayoutManager("TooBad", 1);
    initEffectKeeper(0, NULL, NULL);
    initNerve(&MissLayoutNrvWait::sInstance);
    kill();
}

void MissLayout::kill() {
    LayoutActor::kill();
    setNerve(&MissLayoutNrvWait::sInstance);
}

bool MissLayout::isActive() const {
    return isNerve(&MissLayoutNrvActive::sInstance);
}

void MissLayout::start() {
    if (isNerve(&MissLayoutNrvActive::sInstance)) {
        return;
    }

    LayoutActor::appear();
    MR::hideLayout(this);
    setNerve(&MissLayoutNrvActive::sInstance);
}

void MissLayout::exeWait() {
    
}

void MissLayout::exeActive() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        MR::startAnim(this, "Appear", 0);
    }

    if (MR::isStep(this, 18)) {
        MR::emitEffect(this, "TooBad");
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&MissLayoutNrvEnd::sInstance);
    }
}

void MissLayout::exeEnd() {
    
}

MissLayout::~MissLayout() {
    
}
