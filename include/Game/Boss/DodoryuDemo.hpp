#pragma once

#include "Game/Boss/DodoryuStateBase.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"

class DodoryuDemoOpening : public DodoryuStateBase {
public:
    DodoryuDemoOpening(Dodoryu* pHost, const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x48 */ virtual void control();

    /* 0x78 */ virtual void start();
    /* 0x7C */ virtual void end();

    bool tryFinish();
    void turnToRabbit();

    void exeDemoWait();
    void exeUnderground();
    void exeUndergroundHide();
    void exeAppear();
    void exeWalk();
    void exeHideReady();
    void exeHide();

    /* 0x90 */ ActorCameraInfo mActorCameraInfo;
};

class DodoryuDemoAppear : public DodoryuStateBase {
public:
    DodoryuDemoAppear(Dodoryu* pHost, const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);

    /* 0x78 */ virtual void start();
    /* 0x7C */ virtual void end();

    void exeDemoWait();
    void exePlay();

    /* 0x90 */ ActorCameraInfo mActorCameraInfo;
};

class DodoryuDemoAngry : public DodoryuStateBase {
public:
    DodoryuDemoAngry(Dodoryu* pHost, const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);

    /* 0x78 */ virtual void start();
    /* 0x7C */ virtual void end();

    void exeDemoWait();
    void exePlay();

    /* 0x90 */ ActorCameraInfo mActorCameraInfo;
};

class DodoryuDemoDown : public DodoryuStateBase {
public:
    DodoryuDemoDown(Dodoryu* pHost, const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);

    /* 0x78 */ virtual void start();
    /* 0x7C */ virtual void end();

    void exeDemoWait();
    void exePlay();

    /* 0x90 */ ActorCameraInfo mActorCameraInfo;
};
