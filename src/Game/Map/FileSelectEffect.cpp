#include "Game/Map/FileSelectEffect.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    NEW_NERVE(FileSelectEffectNrvAppear, FileSelectEffect, Appear);
    NEW_NERVE(FileSelectEffectNrvWait, FileSelectEffect, Wait);
    NEW_NERVE(FileSelectEffectNrvDisappear, FileSelectEffect, Disappear);
};  // namespace

FileSelectEffect::FileSelectEffect(const char* pName) : LiveActor(pName) {
    mEffectFrame = 0.0f;
}

void FileSelectEffect::init(const JMapInfoIter& rIter) {
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

void FileSelectEffect::exeWait() {
}

void FileSelectEffect::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Disappear");
        MR::setBrkFrame(this, MR::getBrkCtrl(this)->getEnd() - mEffectFrame);
    }

    if (MR::isBrkOneTimeAndStopped(this)) {
        kill();
    }
}

// FileSelectEffect::calcAndSetBaseMtx

FileSelectEffect::~FileSelectEffect() {
}
