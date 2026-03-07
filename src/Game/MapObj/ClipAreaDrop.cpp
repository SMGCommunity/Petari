#include "Game/MapObj/ClipAreaDrop.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/ClipArea.hpp"
#include "Game/MapObj/ClipAreaShape.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

namespace NrvClipAreaDrop {
    NEW_NERVE(ClipAreaDropNrvWait, ClipAreaDrop, Wait);
}

ClipAreaDrop::ClipAreaDrop(const char* pName) : ClipArea(pName) {
    _C0 = 0;
    _C4 = 0.0f;
    setShape(_C0 = new ClipAreaShapeSphere);
}

void ClipAreaDrop::init(const JMapInfoIter& rIter) {
    initBaseMatrix(rIter);
    MR::connectToScene(this, MR::MovementType_ClippedMapParts, -1, -1, MR::DrawType_ClipArea);
    MR::invalidateClipping(this);
    initNerve(&NrvClipAreaDrop::ClipAreaDropNrvWait::sInstance);
    makeActorDead();
}

void ClipAreaDrop::appear() {
    LiveActor::appear();
    _C0->mRadius = 0.0f;
    setNerve(&NrvClipAreaDrop::ClipAreaDropNrvWait::sInstance);
}

void ClipAreaDrop::control() {
    TMtx34f mtx;
    mtx.identity();
    mtx[0][3] = mPosition.x;
    mtx[1][3] = mPosition.y;
    mtx[2][3] = mPosition.z;
    mBaseMatrix.setInline(mtx);
}

void ClipAreaDrop::setBaseSize(f32 v1) {
    _C4 = v1;
}

void ClipAreaDrop::exeWait() {
    f32 f = 0;
    if (MR::isLessStep(this, 15)) {
        f = MR::calcNerveEaseOutValue(this, 15, 0.0f, _C4);
    } else {
        f = MR::calcNerveEaseInOutValue(this, 0x3C, 0xF0, _C4, 0.0);
    }
    _C0->mRadius = f;
    if (MR::isGreaterStep(this, 240)) {
        kill();
    }
}

ClipAreaDrop::~ClipAreaDrop() {}
