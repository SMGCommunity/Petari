#include "Game/Animation/BckCtrl.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util.hpp"
#include "Game/Util/JMapInfo.hpp"

namespace {
    static const char* sDefaultPlayDataName = "_default";
};  // namespace

BckCtrl::BckCtrl(ResourceHolder* pResHolder, const char* pResName) : mControlData(nullptr), mControlDataCount(0), _1C(0) {
    mDefaultCtrlData.mName = sDefaultPlayDataName;
    s32 numCtrl = 0;
    JMapInfo info;
    if (pResHolder->mBanmtResTable->isExistRes(pResName)) {
        info.attach(pResHolder->mBanmtResTable->getRes(pResName));
        numCtrl = info.getNumEntries();
    }

    if (numCtrl > 0) {
        mControlData = new BckCtrlData[numCtrl];
        mControlDataCount = numCtrl;
    }

    if (info.mData != nullptr) {
        for (int i = 0; i < info.getNumEntries(); i++) {
            JMapInfoIter iter(&info, i);

            const char* name = "";
            iter.getValue("name", &name);
            s32 interpole = -1;
            iter.getValue("interpole", &interpole);
            s32 playFrame = -1;
            iter.getValue("play_frame", &playFrame);
            s32 startFrame = -1;
            iter.getValue("start_frame", &startFrame);
            s32 endFrame = -1;
            iter.getValue("end_frame", &endFrame);
            s32 attribute = -1;
            iter.getValue("attribute", &attribute);

            BckCtrlData data;
            data.mName = name;
            data.mInterpole = interpole;
            data.mPlayFrame = playFrame;
            data.mStartFrame = startFrame;
            data.mEndFrame = endFrame;
            data.mRepeatFrame = -1;

            data.mLoopMode = attribute >= 0 ? (u8)attribute : 0xFF;

            overWrite(data);
        }
    }
}

void BckCtrl::overWrite(const BckCtrlData& rNew) {
    if (MR::isEqualStringCase(rNew.mName, ::sDefaultPlayDataName)) {
        mDefaultCtrlData = rNew;
    } else {
        BckCtrlData* data = find(rNew.mName);

        if (data != nullptr) {
            *data = rNew;
        } else {
            add(rNew);
        }
    }
}

void BckCtrl::changeBckSetting(const char* pName, XanimePlayer* pPlayer) const {
    BckCtrlData* data = find(pName);

    if (data != nullptr) {
        bool isValidReflect = data->mInterpole >= 0 || data->mPlayFrame >= 0 || data->mStartFrame >= 0 || data->mEndFrame >= 0 ||
                              data->mRepeatFrame >= 0 || data->mLoopMode != 0xFF;

        if (isValidReflect) {
            BckCtrlFunction::reflectBckCtrlData(*data, pPlayer);
            return;
        }
    }

    BckCtrlFunction::reflectBckCtrlData(mDefaultCtrlData, pPlayer);
}

// BckCtrl::find
// BckCtrlFunction::reflectBckCtrlData
