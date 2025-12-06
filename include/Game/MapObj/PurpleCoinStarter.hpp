#pragma once

#include "Game/Map/StageSwitch.hpp"
#include "Game/MapObj/PurpleCoinHolder.hpp"
#include "Game/NameObj/NameObj.hpp"

class PurpleCoinStarter : public NameObj {
public:
    PurpleCoinStarter(const char*);

    virtual ~PurpleCoinStarter();
    virtual void init(const JMapInfoIter&);

    void setHost(PurpleCoinHolder*);
    void start();

    PurpleCoinHolder* mCoinHolder;  // 0xC
    StageSwitchCtrl* mSwitchCtrl;   // 0x10
};