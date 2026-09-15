#include "Game/Map/FileSelectModel.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    static const f32 sScale = 30.0f;
};  // namespace

namespace {
    NEW_NERVE(FileSelectModelNrvOpen, FileSelectModel, Open);
    NEW_NERVE(FileSelectModelNrvBlinkOnce, FileSelectModel, BlinkOnce);
    NEW_NERVE(FileSelectModelNrvClose, FileSelectModel, Close);
    NEW_NERVE(FileSelectModelNrvBlink, FileSelectModel, Blink);
};  // namespace

FileSelectModel::FileSelectModel(const char* pModelName, MtxPtr pHostMtx, const char* pName) : LiveActor(pName), _8C(pHostMtx) {
    initModelManagerWithAnm(pModelName, nullptr, false);
    MR::connectToSceneNpc(this);
    initEffectKeeper(0, nullptr, false);
    MR::initLightCtrl(this);
    mScale.set(::sScale, ::sScale, ::sScale);
    initNerve(GET_NERVE_ANON(FileSelectModelNrvOpen));
    MR::invalidateClipping(this);
    makeActorDead();
}

void FileSelectModel::calcAnim() {
    LiveActor::calcAnim();
}

void FileSelectModel::open() {
    if (isNerve(GET_NERVE_ANON(FileSelectModelNrvOpen))) {
        return;
    }

    setNerve(GET_NERVE_ANON(FileSelectModelNrvOpen));
}

void FileSelectModel::blinkOnce() {
    setNerve(GET_NERVE_ANON(FileSelectModelNrvBlinkOnce));
}

void FileSelectModel::close() {
    setNerve(GET_NERVE_ANON(FileSelectModelNrvClose));
}

void FileSelectModel::blink() {
    setNerve(GET_NERVE_ANON(FileSelectModelNrvBlink));
}

bool FileSelectModel::isOpen() const {
    return isNerve(GET_NERVE_ANON(FileSelectModelNrvOpen));
}

void FileSelectModel::emitOpen() {
    MR::emitEffect(this, "Open");
}

void FileSelectModel::emitVanish() {
    MR::emitEffect(this, "Vanish");
}

void FileSelectModel::emitCopy() {
    MR::emitEffect(this, "Copy");
}

void FileSelectModel::emitCompleteEffect() {
    MR::emitEffect(this, "Complete");
}

void FileSelectModel::deleteCompleteEffect() {
    MR::deleteEffect(this, "Complete");
}

void FileSelectModel::exeOpen() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "normal");
    }
}

void FileSelectModel::exeBlinkOnce() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "blink");
    }

    if (MR::isBtpStopped(this)) {
        setNerve(GET_NERVE_ANON(FileSelectModelNrvOpen));
    }
}

void FileSelectModel::exeClose() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "close");
    }
}

void FileSelectModel::exeBlink() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "open");
    }

    if (MR::isBtpStopped(this)) {
        setNerve(GET_NERVE_ANON(FileSelectModelNrvOpen));
    }
}

void FileSelectModel::calcAndSetBaseMtx() {
    mPosition.setTrans(_8C);
    MR::setBaseTRMtx(this, _8C);
}
