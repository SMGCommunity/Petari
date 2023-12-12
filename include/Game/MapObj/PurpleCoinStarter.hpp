#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/Map/StageSwitch.h"
#include "Game/MapObj/PurpleCoinHolder.h"

class PurpleCoinStarter : public NameObj {
public:
    PurpleCoinStarter(const char *);

    virtual ~PurpleCoinStarter();
    virtual void init(const JMapInfoIter &);

    void setHost(PurpleCoinHolder *);
    void start();

    PurpleCoinHolder* mCoinHolder;  // _C
    StageSwitchCtrl* mSwitchCtrl;   // _10
};