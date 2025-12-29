#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;
class LiveActorGroup;

class WarpPod : public LiveActor {
    friend class WarpPodMgr;

public:
    /// @brief Creates a new `WarpPod`.
    /// @param pName A pointer to the null-terminated name of the object.
    WarpPod(const char* pName);

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

private:
    /* 0x8C */ JMapIdInfo* _8C;
    /* 0x90 */ s32 _90;
    /* 0x94 */ ActorCameraInfo* _94;
    /* 0x98 */ char* mEventCameraName;
    /* 0x9C */ char* _9C;
    /* 0xA0 */ u16 _A0;
    /* 0xA2 */ u16 _A2;
    /* 0xA4 */ u16 _A4;
    /* 0xA6 */ u16 _A6;
    /* 0xA8 */ s32 mArg1;
    /* 0xAC */ s32 mArg2;
    /* 0xB0 */ s32 mArg3;
    /* 0xB4 */ s32 mPathFlagIndex;
    /* 0xB8 */ s32 mArg4;
    /* 0xBC */ s32 mArg5;
    /* 0xC0 */ s32 mArg6;
    /* 0xC4 */ TVec3f* _C4;
    /* 0xC8 */ u16 _C8;
    /* 0xCA */ u8 _CA;
    /* 0xCB */ u8 _CB;
    /* 0xCC */ u8 _CC;
    /* 0xCD */ u8 _CD;
    /* 0xD0 */ WarpPod* mPairPod;
    /* 0xD4 */ u8 _D4[8];
};

class WarpPodMgr : public NameObj {
    friend class WarpPod;

public:
    /// @brief Creates a new `WarpPodMgr`.
    /// @param pName A pointer to the null-terminated name of the object.
    WarpPodMgr(const char* pName);

    virtual void draw() const;

    WarpPod* getPairPod(const LiveActor*);
    void startEventCamera(const LiveActor*);
    void endEventCamera();
    void notifyWarpEnd(WarpPod*);

private:
    /* 0x0C */ const LiveActor* _C;
    /* 0x10 */ LiveActorGroup* _10;
    /* 0x14 */ s32 _14;
};

namespace MR {
    WarpPodMgr* getWarpPodManager();
};  // namespace MR
