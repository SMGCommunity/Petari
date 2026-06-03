#include "JSystem/JAudio2/JASVoiceBank.hpp"
#include "JSystem/JAudio2/JASBasicInst.hpp"

const JASOscillator::Data JASVoiceBank::sOscData = {
    0, 1.0f, nullptr, nullptr, 1.0f, 0.0f,
};

JASOscillator::Data* JASVoiceBank::sOscTable;

bool JASVoiceBank::getInstParam(int param_0, int param_1, int param_2, JASInstParam* param_3) const {
    if (param_0 < 0) {
        return false;
    }
    sOscTable = (JASOscillator::Data*)&sOscData;
    param_3->_18 = param_0;
    param_3->mNumOscillators = 1;
    param_3->mOscillatorData = &sOscTable;
    return true;
}
