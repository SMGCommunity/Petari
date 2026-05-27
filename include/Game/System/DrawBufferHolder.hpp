#pragma once

#include "Game/System/DrawBufferGroup.hpp"
#include "Game/Util.hpp"

class LiveActor;

typedef MR::Vector< MR::AssignableArray< DrawBufferGroup* > > ExecutorList;

struct DrawBufferInitialTable {
    /* 0x00 */ u32 mDrawBufferType;
    /* 0x04 */ s32 mCapacity;
    /* 0x08 */ u32 mLightType;
    /* 0x0C */ u32 mDrawCameraType;
};

class DrawBufferHolder {
public:
    DrawBufferHolder();

    void initTable(const DrawBufferInitialTable*, s32);

    void allocateActorListBuffer();
    void active(LiveActor*, s32, s32);
    s32 registerDrawBuffer(LiveActor*, s32);
    void deactive(LiveActor*, s32, s32);
    void findLightInfo(LiveActor*, s32, s32);
    void entry(s32);

    void drawOpa(s32) const;
    void drawXlu(s32) const;

    ExecutorList& getExecuteList(s32 drawBufferType) {
        s32 listId = mBufferGroups[drawBufferType].mDrawCameraType;
        return mExecuteLists[listId];
    }

    bool isBufferGroupEmpty(s32 drawBufferType) const {
        return mBufferGroups[drawBufferType].mActiveExecutors.size() == 0;
    }

    DrawBufferGroup* getDrawBufferGroup(s32 drawBufferType) {
        return &mBufferGroups[drawBufferType];
    }

    // debug symbol map functions
    s32 getActiveActorCount(const char*);
    void onEntryAll();
    void offEntryAll();
    void onDrawOpaAll();
    void offDrawOpaAll();
    void onDrawXluAll();
    void offDrawXluAll();

    // TODO: see .cpp
    void dummy(s32);

    /* 0x00 */ DrawBufferGroup* mBufferGroups;
    /* 0x04 */ s32 mBufferGroupCount;
    /* 0x08 */ ExecutorList mExecuteLists[3];
    /* 0x2C */ bool mTableInitialized;
};
