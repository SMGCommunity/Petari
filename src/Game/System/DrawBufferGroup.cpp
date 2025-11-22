#include "Game/System/DrawBufferGroup.hpp"

#include <algorithm>

DrawBufferGroup::DrawBufferGroup()
    : _0(), _C() {
    mCount = 0;
    _1C = -1;
    _20 = -1;
}

void DrawBufferGroup::init(s32 count) {
    _0.init(count);
    _C.init(count);
}

s32 DrawBufferGroup::registerDrawBuffer(LiveActor* pActor) {
    const char* pModelName = MR::getModelResName(pActor);
    s32         idx = findExecuterIndex(pModelName);

    // executer does not exist
    if (idx < 0) {
        DrawBufferExecuter* exec = new DrawBufferExecuter(pModelName, MR::getJ3DModel(pActor), 0x10);

        idx = _0.size();
        _0.push_back(exec);

        if (_20 == -1) {
            exec->onExecuteLight(_1C);
        }
    }

    _0[idx]->mDrawBufferCount++;

    return idx;
}

void DrawBufferGroup::active(LiveActor* pActor, s32 a2) {
    DrawBufferExecuter* exec = _0[a2];
    bool                isEmpty = !(exec->_8 != 0);
    exec->add(pActor);

    if (isEmpty) {
        _C.push_back(_0[a2]);
    }
}

// DrawBufferGroup::deactive

void DrawBufferGroup::findLightInfo(LiveActor* pActor, s32 a2) {
    MR::initActorLightInfoLightType(pActor, _1C);
    _0[a2]->findLightInfo(pActor);

    if (_20 != -1) {
        for (u32 i = 0; i < _0.size(); i++) {
            _0[i]->onExecuteLight(_1C);
        }

        _20 = -1;
    }

    _0[a2]->offExecuteLight();
}

#ifdef NON_MATCHING
// mem_fun doesn't get inlined...why?
void DrawBufferGroup::entry() {
    for_each(_C.begin(), _C.end(), mem_fun(&DrawBufferExecuter::calcViewAndEntry));
}
#endif

// DrawBufferGroup::drawOpa
// DrawBufferGroup::drawXlu

void DrawBufferGroup::setDrawCameraType(s32 type) {
    mCount = type;
}

void DrawBufferGroup::setLightType(s32 type) {
    _1C = type;
    _20 = type;
}

s32 DrawBufferGroup::findExecuterIndex(const char* pName) const {
    for (u32 i = 0; i < _0.size(); i++) {
        if (MR::isEqualString(_0[i]->mName, pName)) {
            return i;
        }
    }

    return -1;
}
