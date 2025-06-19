#include "Game/MapObj/ClipAreaDrop.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/ClipArea.hpp"
#include "Game/MapObj/ClipAreaShape.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "revolution/types.h"

namespace NrvClipAreaDrop {
    NEW_NERVE(ClipAreaDropNrvWait, ClipAreaDrop, Wait);
}

ClipAreaDrop::ClipAreaDrop(const char* pName) : ClipArea(pName) {
    _C0 = 0;
    _C4 = 0.0f;
    setShape(new ClipAreaShapeSphere);
}

void ClipAreaDrop::init(const JMapInfoIter& rIter) {
    initBaseMatrix(rIter);
    MR::connectToScene(this, 28, -1, -1, 42);
    MR::invalidateClipping(this);
    initNerve(&NrvClipAreaDrop::ClipAreaDropNrvWait::sInstance);
    makeActorDead();
}

void ClipAreaDrop::setBaseSize(f32 v1) {
    _C4 = v1;
}

void ClipAreaDrop::appear() {
    LiveActor::appear();
    setNerve(&NrvClipAreaDrop::ClipAreaDropNrvWait::sInstance);
}

void ClipAreaDrop::exeWait() {
    if (MR::isLessStep(this, 15)) {
        MR::calcNerveEaseOutValue(this, 15, 0.0f, _C4);
    }
    else {
        MR::calcNerveEaseInOutValue(this, 60, _C4, 0.0f, 0);
    }
    if (MR::isGreaterStep(this, 240)) {
        kill();
    }
}

ClipAreaDrop::~ClipAreaDrop() {}
