#include "Game/System/DrawBufferGroup.hpp"

#include <algorithm>

DrawBufferGroup::DrawBufferGroup() : _0(), _C() {
    mCount = 0;
    _1C = -1;
    _20 = -1;
}

void DrawBufferGroup::init(s32 count) {
    _0.mArray.mArr = new DrawBufferExecuter*[count];
    _0.mArray.mMaxSize = count;
    _C.mArray.mArr = new DrawBufferExecuter*[count];
    _C.mArray.mMaxSize = count;
}

s32 DrawBufferGroup::registerDrawBuffer(LiveActor *pActor) {
    const char* modelName = MR::getModelResName(pActor);
    s32 idx = findExecuterIndex(modelName);

    // executer does not exist
    if (idx < 0) {
        DrawBufferExecuter* exec = new DrawBufferExecuter(modelName, MR::getJ3DModel(pActor), 0x10);

        idx = _0.mCount;
        _0.push_back(exec);

        if (_20 == -1) {
            exec->onExecuteLight(_1C);
        }
    }

    _0.mArray.mArr[idx]->mDrawBufferCount++;

    return idx;
}

void DrawBufferGroup::active(LiveActor *pActor, s32 a2) {
    DrawBufferExecuter* exec = _0.mArray.mArr[a2];
    bool isEmpty = !(exec->_8 != 0);
    exec->add(pActor);

    if (isEmpty) {
        _C.push_back(_0.mArray.mArr[a2]);
    }
}

// DrawBufferGroup::deactive

void DrawBufferGroup::findLightInfo(LiveActor *pActor, s32 a2) {
    MR::initActorLightInfoLightType(pActor, _1C);
    _0.mArray.mArr[a2]->findLightInfo(pActor);

    if (_20 != -1) {
        for (u32 i = 0; i < _0.mCount; i++) {
            _0.mArray.mArr[i]->onExecuteLight(_1C);
        }

        _20 = -1;
    }

    _0.mArray.mArr[a2]->offExecuteLight();
}

#ifdef NON_MATCHING
// mem_fun doesn't get inlined...why? 
void DrawBufferGroup::entry() {
    for_each(_C.mArray.mArr, &_C.mArray.mArr[_C.mArray.mMaxSize], mem_fun(&DrawBufferExecuter::calcViewAndEntry));
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

s32 DrawBufferGroup::findExecuterIndex(const char *pName) const {
    for (u32 i = 0; i < _0.mCount; i++) {
        if (MR::isEqualString(_0.mArray.mArr[i]->mName, pName)) {
            return i;
        }
    }

    return -1;
}
