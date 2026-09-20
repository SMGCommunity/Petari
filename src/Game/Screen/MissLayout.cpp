#include "Game/Screen/MissLayout.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    NEW_NERVE(MissLayoutNrvWait, MissLayout, Wait);
    NEW_NERVE(MissLayoutNrvActive, MissLayout, Active);
    NEW_NERVE(MissLayoutNrvEnd, MissLayout, End);
};  // namespace

MissLayout::MissLayout() : LayoutActor("ミス!", true) {
}

void MissLayout::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayout(this);
    initLayoutManager("TooBad", 1);
    initEffectKeeper(0, nullptr, nullptr);
    initNerve(GET_NERVE_ANON(MissLayoutNrvWait));
    kill();
}

void MissLayout::kill() {
    LayoutActor::kill();
    setNerve(GET_NERVE_ANON(MissLayoutNrvWait));
}

bool MissLayout::isActive() const {
    return isNerve(GET_NERVE_ANON(MissLayoutNrvActive));
}

void MissLayout::start() {
    if (isNerve(GET_NERVE_ANON(MissLayoutNrvActive))) {
        return;
    }

    LayoutActor::appear();
    MR::hideLayout(this);
    setNerve(GET_NERVE_ANON(MissLayoutNrvActive));
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
        setNerve(GET_NERVE_ANON(MissLayoutNrvEnd));
    }
}

void MissLayout::exeEnd() {
}
