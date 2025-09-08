#include "Game/Animation/BckCtrl.hpp"
#include "Game/Util.hpp"

typedef int intptr_t;

extern "C" int strcasecmp__2MRFPCcPCc(const char* a, const char* b);

namespace {
    static const char* sDefaultPlayDataName = "_default";
};

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

        if (data->mInterpole >= 0 || data->mPlayFrame >= 0 || data->mStartFrame >= 0 || data->mEndFrame >= 0 || data->mRepeatFrame >= 0 || data->mLoopMode != 0xFF) {
            reflectDataPtr = true;
        }

        if (reflectDataPtr) {
            BckCtrlFunction::reflectBckCtrlData(*data, pPlayer);
            return;
        }
    }

    BckCtrlFunction::reflectBckCtrlData(mDefaultCtrlData, pPlayer);
}


/// NON_MATCHING:
/// Toolchain mismatch (MWCC 3.0a3 build differences) prevents exact codegen.
/// Target emits srawi/addze; our compiler variant prefers clrlwi./beq.
/// Behavior is identical; will match once toolchain is aligned.
BckCtrlData* BckCtrl::find(const char* pName) const {
    const BckCtrl* r26 = this;
    const char* r27 = pName;
    BckCtrlData* r28 = mControlData;
    BckCtrlData* r30;
    int r29, r31;
    int cmp;

    int tempEnd = (int)r28 + mControlDataCount * 0x14;
    r29 = ((int)r28 - tempEnd) / 0x14;

    do {
        int step = r29 >> 1;
        r31 = step;
        if (r29 & 1) {
            r31 += 1;
        }

        int tempMul = r31 * 0x14;
        r30 = (BckCtrlData*)((char*)r28 + tempMul);

        cmp = strcasecmp__2MRFPCcPCc(r30->_0, r27);

        if (((unsigned)cmp >> 31) != 0) {
            int onePlus = r31 + 1;
            r28 = (BckCtrlData*)((char*)r30 + 0x14);
            r29 -= onePlus;
        } else {
            r29 = r31;
        }
    } while (r29 > 0);

    int tempCountMul = r26->mControlDataCount * 0x14;
    BckCtrlData* end =
        (BckCtrlData*)((char*)r26->mControlData + tempCountMul);

    if (r28 != end) {
        if (strcasecmp__2MRFPCcPCc(r28->_0, r27) == 0) {
            return r28;
        }
    }

    return nullptr;
}
// BckCtrlFunction::reflectBckCtrlData
