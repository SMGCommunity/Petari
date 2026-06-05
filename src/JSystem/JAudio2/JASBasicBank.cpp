#include "JSystem/JAudio2/JASBasicBank.hpp"
#include "JSystem/JAudio2/JASCalc.hpp"

JASBasicBank::JASBasicBank() {
    JASCalc::bzero(&mInstTable, INST_NUM_MAX * 4);
}

bool JASBasicBank::getInstParam(int prg_no, int pitch, int velocity, JASInstParam* o_param) const {
    JASInst* inst = getInst(prg_no);
    if (inst == nullptr) {
        return false;
    }
    return inst->getParam(pitch, velocity, o_param);
}

void JASBasicBank::setInst(int prg_no, JASInst* inst) {
    mInstTable[prg_no] = inst;
}

JASInst* JASBasicBank::getInst(int prg_no) const {
    if (prg_no < 0) {
        return nullptr;
    }
    if (prg_no >= INST_NUM_MAX) {
        return nullptr;
    }
    return mInstTable[prg_no];
}
