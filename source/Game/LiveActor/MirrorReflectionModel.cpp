#include "Game/LiveActor/MirrorReflectionModel.h"
#include "Game/Demo/DemoFunction.h"

void MirrorReflectionModel::init(JMapInfoIter const& pMapInfo) {
    MR::invalidateClipping(this);
    DemoFunction::registerDemoSimpleCastAllFunction(this);
    makeActorAppeared();
    _94 = 0;
}

/* I'm not sure what to do about the FUN_80262794 call.
void MirrorReflectionModel::movement() {
    if (!MR::isDead(_8C) && !MR::isClipped(_8C) && !MR::isNoEntryDrawBuffer(_8C)) {
        if (_94) {
            _94 = 0;
            if (!MR::isNoEntryDrawBuffer(this)) MR::connectToDrawTemporarily(this);
        }
        LiveActor::movement();
    } else if (!_94) {
        _94 = 1;
        if (!MR::isNoEntryDrawBuffer(this)) {
            FUN_80262794(this);
        }
    }
}*/

void MirrorReflectionModel::calcAnim() {
    if (_94 || MR::isDead(this) || MR::isNoCalcAnim(this) || MR::isClipped(this)) return;
    LiveActor::calcAnim();
}

void MirrorReflectionModel::calcViewAndEntry() {
    if (_94) return;
    LiveActor::calcViewAndEntry();
}

/* I'm not sure how to initialize the temporary correctly.
void MirrorReflectionModel::calcAndSetBaseMtx()
{
    JGeometry::TPosition3<JGeometry::TMatrix34<JGeometry::SMatrix34C<float>>> tmp;
    if (_90 == NULL) {
        LiveActor::calcAndSetBaseMtx();
    } else {
        tmp.getTrans(mPosition);
        MR::setBaseTRMtx(this, _90);
    }
}*/