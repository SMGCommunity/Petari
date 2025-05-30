#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/MiiConfirmIcon.hpp"
#include "Game/Util/LayoutUtil.hpp"

namespace {
    NEW_NERVE(MiiConfirmIconNrvAppear, MiiConfirmIcon, Appear);
    NEW_NERVE(MiiConfirmIconNrvWait, MiiConfirmIcon, Wait);
    NEW_NERVE(MiiConfirmIconNrvDisappear, MiiConfirmIcon, Disappear);
};

MiiConfirmIcon::MiiConfirmIcon(const char* pName) :
    LayoutActor(pName, true)
{}

void MiiConfirmIcon::init(const JMapInfoIter& rIter) {
    initLayoutManager("MiiConfirmIcon", 1);
    initNerve(&MiiConfirmIconNrvAppear::sInstance);
}

void MiiConfirmIcon::appear(nw4r::lyt::TexMap* pTexMap, const wchar_t* pName) {
    MR::replacePaneTexture(this, "ShaMiiDummy", pTexMap, 0);
    MR::replacePaneTexture(this, "PicMiiDummy", pTexMap, 0);
    MR::setTextBoxMessageRecursive(this, "MiiName", pName);
    appear();
    setNerve(&MiiConfirmIconNrvAppear::sInstance);
}

void MiiConfirmIcon::disappear() {
    setNerve(&MiiConfirmIconNrvDisappear::sInstance);
}

void MiiConfirmIcon::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "ButtonAppear", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&MiiConfirmIconNrvWait::sInstance);
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
    return isNerve(&MiiConfirmIconNrvDisappear::sInstance);
}

void MiiConfirmIcon::appear() {
    LayoutActor::appear();
}

MiiConfirmIcon::~MiiConfirmIcon() {
    
}
