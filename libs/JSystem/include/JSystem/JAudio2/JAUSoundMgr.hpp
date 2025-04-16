#pragma once

#include "JSystem/JAudio2/JAISoundStarter.hpp"
#include <revolution.h>

class JAUSoundMgr : public JAISoundStarter {
public:
    JAUSoundMgr();

    u8 _4[0x810];
};
