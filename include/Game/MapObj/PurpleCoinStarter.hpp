#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/MapObj/PurpleCoinHolder.hpp"

class PurpleCoinStarter : public NameObj {
public:
    PurpleCoinStarter(const char *);

    virtual ~PurpleCoinStarter();
    virtual void init(const JMapInfoIter &);

    void setHost(PurpleCoinHolder *);
    void start();

    PurpleCoinHolder* mCoinHolder;  // 0xC
    StageSwitchCtrl* mSwitchCtrl;   // 0x10
};