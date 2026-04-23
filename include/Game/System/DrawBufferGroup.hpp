#pragma once

#include "Game/System/DrawBufferExecuter.hpp"
#include "Game/Util.hpp"

class LiveActor;

class DrawBufferGroup {
public:
    DrawBufferGroup();

    void init(s32);
    s32 registerDrawBuffer(LiveActor*);
    void allocateActorListBuffer();
    void active(LiveActor*, s32);
    void deactive(LiveActor*, s32);
    void findLightInfo(LiveActor*, s32);
    void entry();
    void drawOpa() const;
    void drawXlu() const;
    void setDrawCameraType(s32);
    void setLightType(s32);
    s32 findExecuterIndex(const char*) const;

    bool isExecutorEmpty(int index) const {
        return mExecutors[index]->mNumActors == 0;
    }

    /* 0x00 */ MR::Vector< MR::AssignableArray< DrawBufferExecuter* > > mExecutors;
    /* 0x0C */ MR::Vector< MR::AssignableArray< DrawBufferExecuter* > > mActiveExecutors;
    /* 0x18 */ s32 mDrawCameraType;
    /* 0x1C */ s32 mLightType;
    /* 0x20 */ s32 mLightLoadType;
};
