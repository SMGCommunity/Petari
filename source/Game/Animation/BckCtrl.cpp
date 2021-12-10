#include "Game/Animation/BckCtrl.h"
#include "Game/Util.h"

void BckCtrl::overWrite(const BckCtrlData &rNew) {
    if (MR::isEqualStringCase(rNew._0, sDefaultPlayDataName)) {
        mDefaultCtrlData = rNew;
    }
    else {
        BckCtrlData* data = find(rNew._0);

        if (data) {
            *data = rNew;
        }
        else {
           add(rNew);
        }
    }
}

void BckCtrl::changeBckSetting(const char *pName, XanimePlayer *pPlayer) const {
    BckCtrlData* data = find(pName);

    if (data) {
        bool reflectDataPtr = false;

        if (data->mInterpole >= 0 || data->mPlayFrame >= 0 || data->mStartFrame >= 0 || data->mEndFrame >= 0 || data->mRepeatFrame >= 0 || data->_E != 0xFF) {
            reflectDataPtr = true;
        }

        if (reflectDataPtr) {
            BckCtrlFunction::reflectBckCtrlData(*data, pPlayer);
            return;
        }
    }

    BckCtrlFunction::reflectBckCtrlData(mDefaultCtrlData, pPlayer);
}

// BckCtrl::find
// BckCtrlFunction::reflectBckCtrlData

BckCtrlData::BckCtrlData() {
    _0 = "";
    mPlayFrame = -1;
    mStartFrame = -1;
    mEndFrame = -1;
    mRepeatFrame = -1;
    mInterpole = -1;
    _E = 0xFF;
}
