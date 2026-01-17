#pragma once

#include "Game/Boss/DinoPackunAction.hpp"

class DinoPackunDemo : public DinoPackunAction {
public:
    DinoPackunDemo(DinoPackun*);

    virtual ~DinoPackunDemo();
    virtual void init();
    virtual void control();

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
