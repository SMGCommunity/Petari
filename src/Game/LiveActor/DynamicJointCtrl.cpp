#include "Game/LiveActor/DynamicJointCtrl.hpp"
#include "Game/Util.hpp"

#ifdef NON_MATCHING
// scheduling issues
JointCtrlRate::JointCtrlRate() {
    _8 = -1;
    _0 = 1.0f;
    _4 = 0;
    _C = -1;
}
#endif

#ifdef NON_MATCHING
// way too many diffs to note here
void JointCtrlRate::update() {
    u32 temp;
    if (_8 < 0) {
       if (_C >= 0) {
            temp = _4;
            _4 = temp - 1;

           if (temp - 1 >= 0) {
               _0 = ((f32)(_4 - 1) / (f32)(_4 - 1));
           }
           else {
               _C = -1;
               _0 = 0.0f;
           }
       }
    }
    else {
        temp = _4;
        _4 = temp - 1;

        if ((temp - 1) >= 0) {
            _0 = ((f32)(_8 - (temp - 1) / _8));
        }
        else {
            _8 = -1;
            _0 = 1.0f;
        }
    }
}
#endif

void JointCtrlRate::startCtrl(s32 val) {
    if (val < 0) {
        val = 0xA;
    }

    _8 = val;
    _C = -1;
    _4 = val;
    _0 = 0.0f;
}

void JointCtrlRate::endCtrl(s32 val) {
    if (val < 0) {
        val = 0xA;
    }

    _8 = val;
    _C = -1;
    _4 = val;
    _0 = 1.0f;
}

void DynamicJointCtrlKeeper::update() {
    for (s32 i = 0; i < _4; i++) {
        mControls[i]->update();
    }
}

void DynamicJointCtrlKeeper::setCallBackFunction() {
    for (s32 i = 0; i < _4; i++) {
        mControls[i]->setCallBackFunction();
    }
}

void DynamicJointCtrlKeeper::startCtrl(const char *pName, s32 a2) {
    findJointCtrl(pName)->mControlRate->startCtrl(a2);
}

void DynamicJointCtrlKeeper::endCtrl(const char *pName, s32 a2) {
    findJointCtrl(pName)->mControlRate->endCtrl(a2);
}

void DynamicJointCtrlKeeper::reset() {
    for (s32 i = 0; i < _4; i++) {
        mControls[i]->reset();
    }
}

DynamicJointCtrl* DynamicJointCtrlKeeper::findJointCtrl(const char *pName) {
    for (s32 i = 0; i < _4; i++) {
        if (MR::isEqualString(mControls[i]->mName, pName)) {
            return mControls[i];
        }
    }

    return mControls[0];
}