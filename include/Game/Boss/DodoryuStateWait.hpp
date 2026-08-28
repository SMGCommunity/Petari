#pragma once

#include "Game/Boss/DodoryuStateBase.hpp"

class DodoryuStateWait : public DodoryuStateBase {
public:
    DodoryuStateWait(Dodoryu* pHost, const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);

    void exeWait();

private:
    /* 0x90 */ s32 mNextStateCounter;
};
