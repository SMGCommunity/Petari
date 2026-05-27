#include "Game/System/DrawBufferGroup.hpp"
#include "Game/System/DrawBufferExecuter.hpp"
#include "Game/Util/LightUtil.hpp"

#include <algorithm>

DrawBufferGroup::DrawBufferGroup() : mExecutors(), mActiveExecutors(), mDrawCameraType(0), mLightType(-1), mLightLoadType(-1) {
}

void DrawBufferGroup::init(s32 count) {
    mExecutors.init(count);
    mActiveExecutors.init(count);
}

s32 DrawBufferGroup::registerDrawBuffer(LiveActor* pActor) {
    const char* pModelName = MR::getModelResName(pActor);
    s32 idx = findExecuterIndex(pModelName);

    if (idx < 0) {
        DrawBufferExecuter* exec = new DrawBufferExecuter(pModelName, MR::getJ3DModel(pActor), 0x10);

        idx = mExecutors.size();
        mExecutors.push_back(exec);

        if (mLightLoadType == -1) {
            exec->onExecuteLight(mLightType);
        }
    }

    mExecutors[idx]->mDrawBufferCount++;

    return idx;
}

void DrawBufferGroup::allocateActorListBuffer() {
    std::for_each(mExecutors.begin(), mExecutors.end(), std::mem_func(&DrawBufferExecuter::allocateActorListBuffer));
}

void DrawBufferGroup::active(LiveActor* pActor, s32 index) {
    bool isEmpty = isExecutorEmpty(index);
    mExecutors[index]->add(pActor);

    if (isEmpty) {
        mActiveExecutors.push_back(mExecutors[index]);
    }
}

void DrawBufferGroup::deactive(LiveActor* pActor, s32 index) {
    mExecutors[index]->remove(pActor);
    if (isExecutorEmpty(index)) {
        mActiveExecutors[std::find(mActiveExecutors.begin(), mActiveExecutors.begin() + mActiveExecutors.size(), mExecutors[index]) -
                         mActiveExecutors.begin()] = mActiveExecutors[mActiveExecutors.size() - 1];
        mActiveExecutors.mCount--;
    }
}

void DrawBufferGroup::findLightInfo(LiveActor* pActor, s32 index) {
    MR::initActorLightInfoLightType(pActor, mLightType);
    mExecutors[index]->findLightInfo(pActor);

    if (mLightLoadType != -1) {
        for (u32 i = 0; i < mExecutors.size(); i++) {
            mExecutors[i]->onExecuteLight(mLightType);
        }

        mLightLoadType = -1;
    }

    mExecutors[index]->offExecuteLight();
}

void DrawBufferGroup::entry() {
    if (mActiveExecutors.size() != 0) {
        std::for_each(mActiveExecutors.begin(), mActiveExecutors.end(), std::mem_func(&DrawBufferExecuter::calcViewAndEntry));
    }
}

void DrawBufferGroup::drawOpa() const {
    if (mActiveExecutors.size() != 0) {
        j3dSys.mDrawMode = 3;
        if (mLightLoadType != -1) {
            MR::loadLight(mLightLoadType);
        }
        std::for_each(mActiveExecutors.begin(), mActiveExecutors.end(), std::const_mem_func(&DrawBufferExecuter::drawOpa));
    }
}

void DrawBufferGroup::drawXlu() const {
    if (mActiveExecutors.size() != 0) {
        j3dSys.mDrawMode = 4;
        if (mLightLoadType != -1) {
            MR::loadLight(mLightLoadType);
        }
        std::for_each(mActiveExecutors.begin(), mActiveExecutors.end(), std::const_mem_func(&DrawBufferExecuter::drawXlu));
    }
}

void DrawBufferGroup::setDrawCameraType(s32 type) {
    mDrawCameraType = type;
}

void DrawBufferGroup::setLightType(s32 type) {
    mLightType = type;
    mLightLoadType = type;
}

s32 DrawBufferGroup::findExecuterIndex(const char* pName) const {
    for (u32 i = 0; i < mExecutors.size(); i++) {
        if (MR::isEqualString(mExecutors[i]->mName, pName)) {
            return i;
        }
    }

    return -1;
}
