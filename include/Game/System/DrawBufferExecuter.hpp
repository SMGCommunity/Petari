#pragma once

#include "revolution.h"

class J3DModel;
class LiveActor;
class DrawBuffer;

class DrawBufferExecuter {
public:
    DrawBufferExecuter(const char*, J3DModel*, s32);

    void allocateActorListBuffer();
    void add(LiveActor*);
    void remove(LiveActor*);
    void findLightInfo(LiveActor*);
    void onExecuteLight(s32);
    void offExecuteLight();
    void calcViewAndEntry();
    void drawOpa() const;
    void drawXlu() const;

    LiveActor* getActor(int index) const {
        return mActors[index];
    }

    /* 0x00 */ LiveActor** mActors;
    /* 0x04 */ s32 mMaxNumActors;
    /* 0x08 */ s32 mNumActors;
    /* 0x0C */ const char* mName;
    /* 0x10 */ DrawBuffer* mDrawBuffer;
    /* 0x14 */ s32 mLightType;
    /* 0x18 */ s32 mDrawBufferCount;
};
