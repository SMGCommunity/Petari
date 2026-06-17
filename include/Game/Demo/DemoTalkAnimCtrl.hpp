#pragma once

#include "Game/System/NerveExecutor.hpp"
#include <revolution/types.h>

class ActorCameraInfo;
class BckCtrlData;
class JMapInfoIter;
class LiveActor;

class DemoTalkAnimCtrl : public NerveExecutor {
public:
    DemoTalkAnimCtrl(LiveActor*, const char*, const char*);

    void initForScene(const char*, const char*, const JMapInfoIter&);
    void updateCamera();
    void createBckCtrlData(BckCtrlData*, s32) const;
    void initForActor(const char*);
    void startDemo();
    bool updateDemo();
    void startAnim();
    void startCamera();
    void updateAnim(const BckCtrlData&);
    void updatePause();
    void endCamera();
    void setupStartDemoPart(const char*);

    LiveActor* mActor;  // 0x8
    const char* _C;
    const char* _10;
    const char* _14;
    const char* _18;
    const char* _1C;
    ActorCameraInfo* mCameraInfo;  // 0x20
    s32 _24;
    s32 _28;
    s32 _2C;
    s32 _30;
    bool _34;
    bool _35;
    bool _36;
    s32 _38;
    s32 _3C;
    s32 _40;
    s32 _44;
    bool _48;
    bool mHaveCamera;
    bool _4A;
    bool mHaveBtk;  // 0x4B
    bool mHaveBpk;  // 0x4C
    bool mHaveBtp;  // 0x4D
    bool mHaveBrk;  // 0x4E
    bool mHaveBva;  // 0x4F
};
