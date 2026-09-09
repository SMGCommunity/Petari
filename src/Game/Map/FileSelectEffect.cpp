#include "Game/Map/FileSelectEffect.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    NEW_NERVE(FileSelectEffectNrvAppear, FileSelectEffect, Appear);
    NEW_NERVE(FileSelectEffectNrvWait, FileSelectEffect, Wait);
    NEW_NERVE(FileSelectEffectNrvDisappear, FileSelectEffect, Disappear);
};  // namespace

FileSelectEffect::FileSelectEffect(const char* pName) : LiveActor(pName), mEffectFrame() {
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

void FileSelectEffect::disappear() {
    if (MR::isDead(this) || isNerve(&::FileSelectEffectNrvDisappear::sInstance)) {
        return;
    }

    if (isNerve(&::FileSelectEffectNrvAppear::sInstance)) {
        if (MR::isNewNerve(this)) {
            kill();
            return;
        }

        mEffectFrame = MR::getBrkCtrl(this)->getFrame();
    } else if (isNerve(&::FileSelectEffectNrvWait::sInstance)) {
        mEffectFrame = MR::getBrkCtrl(this)->getEnd();
    }

    setNerve(&::FileSelectEffectNrvDisappear::sInstance);
}

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

void FileSelectEffect::calcAndSetBaseMtx() {
    TVec3f dirZ = MR::getCamPos() - mPosition;

    if (MR::isNearZero(dirZ)) {
        return;
    }

    MR::normalize(&dirZ);

    TVec3f dirY = MR::getCamYdir();

    TVec3f dirX;
    dirX.cross(dirY, dirZ);

    if (MR::isNearZero(dirX)) {
        return;
    }

    MR::normalize(&dirX);
    dirY.cross(dirZ, dirX);

    TPos3f mtx;
    mtx.setXYZDir(dirX, dirY, dirZ);
    mtx.setTrans(mPosition);

    MR::setBaseTRMtx(this, mtx);
}
