#include "Game/LiveActor/MirrorReflectionModel.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"

MirrorReflectionModel::MirrorReflectionModel(const LiveActor *pActor, const char *pName, const char *pInternalName, MtxPtr mtx) : LiveActor(pName) {
    _8C = pActor;
    _90 = mtx;
    _94 = 0;

    if (mtx) {
        TPos3f pos;
        pos.set(mtx);
        pos.getTrans(mPosition);
    }
    else {
        mPosition.set<f32>(pActor->mPosition);
    }

    mRotation.set<f32>(_8C->mRotation);
    mScale.set<f32>(_8C->mScale);
    initModelManagerWithAnm(pInternalName, nullptr, false);
    MR::connectToSceneMirrorMapObjDecoration(this);
}

void MirrorReflectionModel::init(const JMapInfoIter &rIter) {
    MR::invalidateClipping(this);
    MR::registerDemoSimpleCastAll(this);
    makeActorAppeared();
    _94 = 0;
}

void MirrorReflectionModel::movement() {
    if (MR::isDead(_8C) || MR::isClipped(_8C) || MR::isHiddenModel(_8C)) {
        if (!_94) {
            _94 = 1;

            if (!MR::isHiddenModel(this)) {
                MR::disconnectToDrawTemporarily(this);
            }
        }
    }
    else {
        if (_94) {
            _94 = 0;
            if (!MR::isHiddenModel(this)) {
                MR::connectToDrawTemporarily(this);
            }
        }

        LiveActor::movement();
    }
}

void MirrorReflectionModel::calcAnim() {
    if (!_94) {
        if (!MR::isDead(this)) {
            if (MR::isNoCalcAnim(this) || MR::isClipped(this)) {
                return;
            }

            LiveActor::calcAnim();
        }
    }
}

void MirrorReflectionModel::calcViewAndEntry() {
    if (!_94) {
        LiveActor::calcViewAndEntry();
    }
}

void MirrorReflectionModel::calcAndSetBaseMtx() {
    if (!_90) {
        LiveActor::calcAndSetBaseMtx();
    }
    else {
        TPos3f pos;
        pos.set(_90);
        pos.getTrans(mPosition);
        MR::setBaseTRMtx(this, _90);
    }
}

MirrorReflectionModel::~MirrorReflectionModel() {

}
