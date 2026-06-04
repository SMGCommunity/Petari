#pragma once

#include "JSystem/JAudio2/JASBank.hpp"
#include "JSystem/JAudio2/JASOscillator.hpp"

class JASVoiceBank : public JASBank {
public:
    virtual ~JASVoiceBank(){};
    virtual bool getInstParam(int, int, int, JASInstParam*) const;
    virtual u32 getType() const {
        return 'VOIC';
    }

    static const JASOscillator::Data sOscData;
    static JASOscillator::Data* sOscTable;
};
