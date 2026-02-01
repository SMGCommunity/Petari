#pragma once

#include "Game/Boss/PoltaActionBase.hpp"

class Polta;

class PoltaDemo : public PoltaActionBase {
public:
	PoltaDemo(Polta*);

    virtual ~PoltaDemo();
	virtual void init();
	virtual void control();

    void startDemoSetting();
    void startOpeningDemo();
    void startPowerUpDemo();
    void startDownDemo();
    void startTryDemo(const char*, const Nerve*);

    void exeTryStartDemo();
    void exeOpeningDemo();
    void exePowerUpDemo();
    void exeDownDemo();

	const Nerve* mCurDemoNerve;
	const char* mCurDemoName;
};
