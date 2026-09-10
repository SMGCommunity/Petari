#include "Game/MapObj/ClipAreaDrop.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/ClipArea.hpp"
#include "Game/MapObj/ClipAreaShape.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

void ClipAreaDrop_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

namespace {
    static const s32 sExtendsEndTime = 15;
    static const s32 sShrinkStartTime = 60;
    static const s32 sShrinkEndTime = 240;
};  // namespace

namespace NrvClipAreaDrop {
    NEW_NERVE(ClipAreaDropNrvWait, ClipAreaDrop, Wait);
};  // namespace NrvClipAreaDrop

ClipAreaDrop::ClipAreaDrop(const char* pName) : ClipArea(pName), mShapeSphere(), mBaseSize(500.0f) {
    mShapeSphere = new ClipAreaShapeSphere();

    setShape(mShapeSphere);
}

void ClipAreaDrop::init(const JMapInfoIter& rIter) {
    initBaseMatrix(rIter);
    MR::connectToScene(this, MR::MovementType_ClippedMapParts, -1, -1, MR::DrawType_ClipArea);
    MR::invalidateClipping(this);
    initNerve(&NrvClipAreaDrop::ClipAreaDropNrvWait::sInstance);
    makeActorDead();
}

void ClipAreaDrop::setBaseSize(f32 baseSize) {
    mBaseSize = baseSize;
}

void ClipAreaDrop::appear() {
    LiveActor::appear();

    mShapeSphere->mRadius = 0.0f;

    setNerve(&NrvClipAreaDrop::ClipAreaDropNrvWait::sInstance);
}

void ClipAreaDrop::control() {
    TPos3f mtx;
    mtx.identity();
    mtx.setTrans(mPosition);

    mBaseMtx.set(mtx);
}

void ClipAreaDrop::exeWait() {
    f32 radius = 0.0f;

    if (MR::isLessStep(this, ::sExtendsEndTime)) {
        radius = MR::calcNerveEaseOutValue(this, ::sExtendsEndTime, 0.0f, mBaseSize);
    } else {
        radius = MR::calcNerveEaseInOutValue(this, ::sShrinkStartTime, ::sShrinkEndTime, mBaseSize, 0.0f);
    }

    mShapeSphere->mRadius = radius;

    if (MR::isGreaterStep(this, ::sShrinkEndTime)) {
        kill();
    }
}
