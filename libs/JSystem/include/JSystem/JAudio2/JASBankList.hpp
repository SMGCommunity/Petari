#pragma once

#include <revolution/types.h>

class JASBank;

class JASBankList {
public:
    JASBankList(){};

    virtual JASBank* getBank(u32) const = 0;
};
