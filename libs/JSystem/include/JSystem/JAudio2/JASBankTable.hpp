#pragma once

#include <JSystem/JAudio2/JASBank.hpp>
#include <JSystem/JAudio2/JASGlobal.hpp>

#include <revolution/types.h>

template < u32 LEN >
class JASBankTable : public JASBankList {
public:
    JASBankTable() : JASBankList(), mBanks() {
    }

    virtual JASBank* getBank(u32 idx) const {
        return mBanks.get(idx);
    }

    void registBank(u32 idx, JASBank* bank) {
        mBanks.set(idx, bank);
    }

    JASPtrArray< JASBank, LEN > mBanks;
};

class JASDefaultBankTable : public JASBankTable< 256 >, public JASGlobalInstance< JASDefaultBankTable > {
public:
    JASDefaultBankTable() : JASGlobalInstance(true) {
    }
};
