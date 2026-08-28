#pragma once

#include "Game/Boss/DinoPackunAction.hpp"

class DinoPackunDemo : public DinoPackunAction {
public:
    DinoPackunDemo(DinoPackun*);

    /* 0x08 */ virtual ~DinoPackunDemo();
    /* 0x0C */ virtual void init();
    /* 0x1C */ virtual void control();

    void startOpeningDemo();
    void startCryDemo();
    void startAngryDemo();
    void startDownDemo();
    void startTryDemo(const char*, const Nerve*);
    void exeTryStartDemo();
    void exeOpeningDemo();
    void exeCryDemo();
    void exeAngryDemo();
    void exeDownDemo();
    void exeAppearPowerStar();

    const Nerve* _14;
    const char* mDemoName;  // 0x18
};
