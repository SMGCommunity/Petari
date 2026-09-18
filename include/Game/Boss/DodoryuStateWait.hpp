#pragma once

#include "Game/Boss/DodoryuStateBase.hpp"

class DodoryuStateWait : public DodoryuStateBase {
public:
    DodoryuStateWait(Dodoryu* pHost, const char* pName);

    virtual void init(const JMapInfoIter& rIter);

    void exeWait();

public:
    /* 0x90 */ s32 mNextStateCounter;
};
