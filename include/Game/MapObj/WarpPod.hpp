#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class WarpPod;
class ActorCameraInfo;
class JMapIdInfo;
class LiveActorGroup;
class JUTTexture;

class WarpPodMgr : public NameObj {
public:
    /// @brief Creates a new `WarpPodMgr`.
    /// @param pName A pointer to the null-terminated name of the object.
    WarpPodMgr(const char* pName);

    virtual void draw() const;

    WarpPod* getPairPod(const LiveActor*);
    void startEventCamera(const LiveActor*);
    void endEventCamera();
    void notifyWarpEnd(WarpPod*);

    /* 0x0C */ const LiveActor* _C;
    /* 0x10 */ LiveActorGroup* _10;
    /* 0x14 */ s32 _14;
};

class WarpPod : public LiveActor {
public:
    /// @brief Creates a new `WarpPod`.
    /// @param pName A pointer to the null-terminated name of the object.
    WarpPod(const char* pName) : LiveActor(pName), mJMapIdInfo(nullptr) {
    }

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void draw() const;
    virtual void appear();
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void glowEffect();
    void initPair();
    void appearWithDemo();
    void startEventCamera() const;
    void endEventCamera();
    void initDraw();
    void drawCylinder(u32) const;

    /* 0x8C */ JMapIdInfo* mJMapIdInfo;
    /* 0x90 */ s32 mGroupId;
    /* 0x94 */ ActorCameraInfo* mCamInfo;
    /* 0x98 */ char* mEventCameraName;
    /* 0x9C */ char* _9C;
    /* 0xA0 */ u16 mDelay;
    /* 0xA2 */ u16 _A2;
    /* 0xA4 */ u16 _A4;
    /* 0xA6 */ u16 _A6;
    /* 0xA8 */ s32 mVisibilityState;
    /* 0xAC */ s32 mArg2;
    /* 0xB0 */ s32 mArg3;
    /* 0xB4 */ s32 mPathFlagIndex;
    /* 0xB8 */ s32 mGrandstarReq;
    /* 0xBC */ s32 mCameraTime;
    /* 0xC0 */ s32 mGlowColorIndex;
    /* 0xC4 */ TVec3f* _C4;
    /* 0xC8 */ u16 _C8;
    /* 0xCA */ bool mArg7;
    /* 0xCB */ bool mIsInactive;
    /* 0xCC */ u8 _CC;
    /* 0xCD */ bool _CD;
    /* 0xD0 */ WarpPod* mPairPod;
    /* 0xD4 */ JUTTexture* _D4;
    /* 0xD8 */ JUTTexture* _D8;
};

namespace MR {
    WarpPodMgr* getWarpPodManager();
};  // namespace MR
