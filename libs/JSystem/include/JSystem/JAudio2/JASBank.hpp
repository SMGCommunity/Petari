#pragma once

#include <JSystem/JAudio2/JASGadget.hpp>
#include <revolution/types.h>

namespace JASDsp {
    struct TChannel;
};

class JASChannel;
struct JASInstParam;
class JASWaveBank;

class JASBank {
public:
    static const int PRG_OSC = 240;

    JASBank() {
        mWaveBank = NULL;
    }
    virtual ~JASBank() {};
    virtual bool getInstParam(int, int, int, JASInstParam*) const = 0;
    virtual u32 getType() const = 0;

    JASWaveBank* getWaveBank() const {
        return mWaveBank;
    }
    void assignWaveBank(JASWaveBank* param_0) {
        mWaveBank = param_0;
    }

    static JASChannel* noteOn(JASBank const*, int, u8, u8, u16, void (*)(u32, JASChannel*, JASDsp::TChannel*, void*), void*) ATTRIBUTE_WEAK;
    static JASChannel* noteOnOsc(int, u8, u8, u16, void (*)(u32, JASChannel*, JASDsp::TChannel*, void*), void*);

private:
    /* 0x04 */ JASWaveBank* mWaveBank;
};

class JASBankList {
public:
    JASBankList() {};

    virtual JASBank* getBank(u32) const = 0;
};
