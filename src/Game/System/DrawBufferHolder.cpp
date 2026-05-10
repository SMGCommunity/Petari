#include "Game/System/DrawBufferHolder.hpp"
#include "Game/System/DrawBufferGroup.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include <algorithm>

namespace {
    // static const char* sDrawTypeCameraName = ;
    // static const char* sPermitMultiCategoryArciveName = ;

    bool isExcludedCheckMultiRegistCategory(s32);
}  // namespace

DrawBufferHolder::DrawBufferHolder() : mBufferGroups(nullptr), mBufferGroupCount(0), mTableInitialized(false) {
}

void DrawBufferHolder::initTable(const DrawBufferInitialTable* pInitialTable, s32 numGroups) {
    // FIXME : regswap and load order
    // https://decomp.me/scratch/RnE7Q

    mBufferGroups = new DrawBufferGroup[numGroups];
    mBufferGroupCount = numGroups;

    s32 cameraTypeCounts[3];
    MR::zeroMemory(cameraTypeCounts, 3 * sizeof(s32));

    const DrawBufferInitialTable* entry;
    for (s32 idx = 0; entry = &pInitialTable[idx], entry->mDrawBufferType != -1; idx++) {
        u32 drawType = entry->mDrawBufferType;
        cameraTypeCounts[entry->mDrawCameraType]++;

        mBufferGroups[drawType].init(entry->mCapacity);
        mBufferGroups[drawType].setDrawCameraType(entry->mDrawCameraType);
        mBufferGroups[drawType].setLightType(entry->mLightType);
    }

    for (s32 idx = 0; idx < 3; idx++) {
        s32 count = cameraTypeCounts[idx];
        if (count > 0) {
            mExecuteLists[idx].init(count);
        }
    }

    mTableInitialized = true;
}

void DrawBufferHolder::allocateActorListBuffer() {
    mTableInitialized = false;
    // NOTE: this symbol does not appear in the debug map, instead it likely uses std::for_each. However the current std::for_each does not support
    // flat array iteration
    std::for_each_array(&mBufferGroups[0], &mBufferGroups[mBufferGroupCount], std::mem_func(&DrawBufferGroup::allocateActorListBuffer));
}

s32 DrawBufferHolder::registerDrawBuffer(LiveActor* pActor, s32 drawBufferType) {
    return mBufferGroups[drawBufferType].registerDrawBuffer(pActor);
}

void DrawBufferHolder::active(LiveActor* pActor, s32 drawBufferType, s32 executorIndex) {
    // FIXME: regswap
    // https://decomp.me/scratch/xttyc

    bool isEmpty = isBufferGroupEmpty(drawBufferType);
    mBufferGroups[drawBufferType].active(pActor, executorIndex);

    if (isEmpty) {
        ExecutorList& executeList = getExecuteList(drawBufferType);
        executeList.push_back(&mBufferGroups[drawBufferType]);
    }
}

void DrawBufferHolder::deactive(LiveActor* pActor, s32 drawBufferType, s32 executorIndex) {
    mBufferGroups[drawBufferType].deactive(pActor, executorIndex);

    if (isBufferGroupEmpty(drawBufferType)) {
        ExecutorList& executeList = getExecuteList(drawBufferType);
        executeList[std::find(executeList.begin(), executeList.end(), &mBufferGroups[drawBufferType]) - executeList.begin()] =
            executeList[executeList.mCount - 1];
        executeList.mCount--;
    }
}

void DrawBufferHolder::findLightInfo(LiveActor* pActor, s32 drawBufferType, s32 executorIndex) {
    mBufferGroups[drawBufferType].findLightInfo(pActor, executorIndex);
}

void DrawBufferHolder::entry(s32 drawBufferType) {
    std::for_each(mExecuteLists[drawBufferType].begin(), mExecuteLists[drawBufferType].end(), std::mem_func(&DrawBufferGroup::entry));
}

void DrawBufferHolder::drawOpa(s32 drawBufferType) const {
    if (isBufferGroupEmpty(drawBufferType)) {
        return;
    }

    mBufferGroups[drawBufferType].drawOpa();
}

void DrawBufferHolder::drawXlu(s32 drawBufferType) const {
    if (isBufferGroupEmpty(drawBufferType)) {
        return;
    }

    mBufferGroups[drawBufferType].drawXlu();
}

void DrawBufferHolder::dummy(s32 drawBufferType) {
    // TODO: This SHOULD NOT be here, this is only here because for_each and Vector<>.end are emitted in this file for DrawBufferGroups,
    // indicating some stripped function uses them. (Check Debug symbols for candidates)
    std::for_each(mExecuteLists[drawBufferType].begin(), mExecuteLists[drawBufferType].end(), std::mem_func(&DrawBufferGroup::entry));
}
