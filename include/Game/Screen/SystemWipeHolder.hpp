#pragma once

#include "Game/Screen/WipeHolderBase.hpp"

class SystemWipeHolder : public WipeHolderBase {
public:
    SystemWipeHolder();
    ~SystemWipeHolder();

    u8 _1C;
};

namespace MR {
    SystemWipeHolder* createSystemWipeHolder();
};
