#include "Game/Map/FileSelectEffect.h"

FileSelectEffect::FileSelectEffect(const char *pName) : LiveActor(pName) {
    mEffectFrame = 0.0f;
}

void FileSelectEffect::init(const JMapInfoIter &rIter) {
    initModelManagerWithAnm("MiniatureGalaxySelect", nullptr, false);
    MR::connectToSceneMapObj(this);
    MR::invalidateClipping(this);
    initNerve(&FileSelectEffectNrvWait::sInstance);
    makeActorDead();
}

void FileSelectEffect::appear() {
    LiveActor::appear();
    setNerve(&FileSelectEffectNrvAppear::sInstance);
}

// FileSelectEffect::disappear

void FileSelectEffect::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Appear");
        MR::startBtk(this, "MiniatureGalaxySelect");
    }

    if (MR::isBrkOneTimeAndStopped(this)) {
        setNerve(&FileSelectEffectNrvWait::sInstance);
    }
}

void FileSelectEffect::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Disappear");
        J3DFrameCtrl* ctrl = MR::getBrkCtrl(this);
        MR::setBrkFrame(this, ctrl->mEndFrame - mEffectFrame);
    }

    if (MR::isBrkOneTimeAndStopped(this)) {
        kill();
    }
}

// FileSelectEffect::calcAndSetBaseMtx

namespace {
    INIT_NERVE(FileSelectEffectNrvAppear);
    INIT_NERVE(FileSelectEffectNrvWait);
    INIT_NERVE(FileSelectEffectNrvDisappear);
};

FileSelectEffect::~FileSelectEffect() {

}
