#pragma once

#include "JSystem/JAudio2/JASBank.hpp"
#include "JSystem/JAudio2/JASGadget.hpp"
#include "JSystem/JSupport/JSUList.hpp"

class JASBank;

class JAUBankTable : public JASBankList {
public:
    JAUBankTable(u32 param_0, JASBank** param_1, u32 param_2) : mBankPtrTable(param_1, param_2) {
        field_0xc = param_0;
    }
    JASBank* getBank(u32 bank) const {
        return mBankPtrTable.get(bank);
    }
    JASBank* getBank(u32 bank) {
        return mBankPtrTable.get(bank);
    }

    void registBank(u32 param_0, JASBank* param_1) {
        mBankPtrTable.set(param_0, param_1);
    }

    /* 0x4 */ JASPtrTable< JASBank > mBankPtrTable;
    /* 0xC */ u32 field_0xc;
};

class JAUBankTableLink : public JSULink< JAUBankTable >, public JAUBankTable {
public:
    JAUBankTableLink(u32 param_0, JASBank** param_1, u32 param_2) : JSULink< JAUBankTable >(this), JAUBankTable(param_0, param_1, param_2) {
    }
};

struct JAUBankTableDictionary : JSUList< JAUBankTable > {
    void appendBankTable(JSULink< JAUBankTable >*);
};

class JASWaveBank;

class JAUWaveBankTable : private JASPtrArray< JASWaveBank, 255 > {
public:
    JASWaveBank* getWaveBank(u32 index) {
        return get(index);
    }
    JASWaveBank* getWaveBank(u32 index) const {
        return get(index);
    }
    void registWaveBank(u32 index, JASWaveBank* bank) {
        set(index, bank);
    }
};
