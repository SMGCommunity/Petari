#include "Game/Screen/MiiConfirmIcon.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LayoutUtil.hpp"

namespace {
    NEW_NERVE(MiiConfirmIconNrvAppear, MiiConfirmIcon, Appear);
    NEW_NERVE(MiiConfirmIconNrvWait, MiiConfirmIcon, Wait);
    NEW_NERVE(MiiConfirmIconNrvDisappear, MiiConfirmIcon, Disappear);
};  // namespace

MiiConfirmIcon::MiiConfirmIcon(const char* pName) : LayoutActor(pName, true) {
}

void MiiConfirmIcon::init(const JMapInfoIter& rIter) {
    initLayoutManager("MiiConfirmIcon", 1);
    initNerve(GET_NERVE_ANON(MiiConfirmIconNrvAppear));
}

void MiiConfirmIcon::appear(nw4r::lyt::TexMap* pTexMap, const wchar_t* pName) {
    MR::replacePaneTexture(this, "ShaMiiDummy", pTexMap, 0);
    MR::replacePaneTexture(this, "PicMiiDummy", pTexMap, 0);
    MR::setTextBoxMessageRecursive(this, "MiiName", pName);
    appear();
    setNerve(GET_NERVE_ANON(MiiConfirmIconNrvAppear));
}

void MiiConfirmIcon::disappear() {
    setNerve(GET_NERVE_ANON(MiiConfirmIconNrvDisappear));
}

void MiiConfirmIcon::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "ButtonAppear", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(GET_NERVE_ANON(MiiConfirmIconNrvWait));
    }
}

void MiiConfirmIcon::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "ButtonWait", 0);
    }
}

void MiiConfirmIcon::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "ButtonEnd", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}

bool MiiConfirmIcon::isDisappear() const {
    return isNerve(GET_NERVE_ANON(MiiConfirmIconNrvDisappear));
}

void MiiConfirmIcon::appear() {
    LayoutActor::appear();
}
