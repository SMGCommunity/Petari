#pragma once

#include "JSystem/JAudio2/JASBank.hpp"
#include "JSystem/JAudio2/JASBasicInst.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"

#define INST_NUM_MAX 0xF0

class JASBasicBank : public JASBank {
public:
    JASBasicBank();

    virtual u32 getType() const {
        return 'BSIC';
    }
    virtual ~JASBasicBank(){};
    virtual bool getInstParam(int, int, int, JASInstParam*) const;

    void newInstTable(u8, JKRHeap*);
    void setInst(int, JASInst*);
    JASInst* getInst(int) const;

    /* 0x8 */ JASInst* mInstTable[INST_NUM_MAX];
};
