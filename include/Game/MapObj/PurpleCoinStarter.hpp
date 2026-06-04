#pragma once

#include "Game/NameObj/NameObj.hpp"

class PurpleCoinHolder;
class StageSwitchCtrl;

class PurpleCoinStarter : public NameObj {
public:
    /// @brief Creates a new `PurpleCoinStarter`.
    /// @param pName A pointer to the null-terminated name of the object.
    PurpleCoinStarter(const char* pName);

    virtual ~PurpleCoinStarter();
    virtual void init(const JMapInfoIter& rIter);

    void setHost(PurpleCoinHolder* pHost);
    void start();

    /* 0x0C */ PurpleCoinHolder* mHost;
    /* 0x10 */ StageSwitchCtrl* mStageSwitchCtrl;
};
