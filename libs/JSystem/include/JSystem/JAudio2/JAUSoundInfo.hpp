#pragma once

#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JAudio2/JAISoundInfo.hpp"
#include "JSystem/JAudio2/JASGlobal.hpp"
#include "JSystem/JAudio2/JAUAudibleParam.hpp"

class JAUSoundInfo : public JASGlobalInstance< JAUSoundInfo > {
public:
    enum DataType {
        DATA_SE = 0x50,
        DATA_BGM = 0x60,
        DATA_STREAM = 0x70,
    };

    JAUSoundInfo(bool);

    virtual u16 getAudibleSw(JAISoundID) const = 0;
    virtual u16 getBgmSeqResourceID(JAISoundID) const = 0;
};
