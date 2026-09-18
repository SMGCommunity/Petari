#include "Game/Animation/BckCtrl.hpp"
#include "Game/Animation/XanimePlayer.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util.hpp"
#include "Game/Util/JMapInfo.hpp"

void BckCtrl_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace {
    static const char* sDefaultPlayDataName = "_default";

    int compareBckName(const BckCtrlData& rFirst, const BckCtrlData& rSecond) {
        if (rFirst.mName[0] == '\0') {
            if (rSecond.mName[0] == '\0') {
                return 0;
            }

            return -1;
        }

        if (rSecond.mName[0] == '\0') {
            if (rFirst.mName[0] == '\0') {
                return 0;
            }

            return 1;
        }

        return MR::strcasecmp(rFirst.mName, rSecond.mName);
    }

    bool isLessBckName(const BckCtrlData& rData, const char* pName) {
        return MR::strcasecmp(rData.mName, pName) < 0;
    }
}  // namespace

BckCtrl::BckCtrl(ResourceHolder* pResHolder, const char* pResName) : mControlData() {
    mDefaultCtrlData.mName = sDefaultPlayDataName;
    s32 numCtrl = 0;
    JMapInfo info;
    if (pResHolder->mBanmtResTable->isExistRes(pResName)) {
        info.attach(pResHolder->mBanmtResTable->getRes(pResName));
        numCtrl = info.getNumEntries();
    }

    if (numCtrl > 0) {
        mControlData.init(numCtrl);
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

void BckCtrl::add(const BckCtrlData& rNew) {
    if (mControlData.capacity() > mControlData.size()) {
        mControlData.push_back(rNew);
        s32 insertIndex = 0;

        for (s32 i = mControlData.size() - 1; i > 0; i--) {
            if (compareBckName(mControlData[i - 1], rNew) < 0) {
                insertIndex = i;
                break;
            }

            mControlData[i] = mControlData[i - 1];
        }

        mControlData[insertIndex] = rNew;
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

BckCtrlData* BckCtrl::find(const char* pName) const {
    s32 half;
    BckCtrlData* pMiddle;
    s32 count;
    BckCtrlData* pFirst;
    pFirst = mControlData.mArray.mArr;
    count = (mControlData.mArray.mArr + mControlData.size()) - pFirst;

    while (count > 0) {
        half = count / 2;
        pMiddle = pFirst + half;

        if (isLessBckName(*pMiddle, pName)) {
            pFirst = pMiddle + 1;
            count -= half + 1;
        } else {
            count = half;
        }
    }

    if (pFirst != mControlData.mArray.mArr + mControlData.size() && MR::strcasecmp(pFirst->mName, pName) == 0) {
        return pFirst;
    }

    return nullptr;
}

inline f32 calcPlayRate(const J3DFrameCtrl& rCtrl, s32 playFrame, f32 baseRate = 1.0f) {
    return baseRate * (static_cast< f32 >(rCtrl.getEnd() - rCtrl.getStart()) / playFrame);
}

void BckCtrlFunction::reflectBckCtrlData(const BckCtrlData& rData, XanimePlayer* pPlayer) {
    XanimeFrameCtrl* pFrameCtrl = pPlayer->getFrameCtrl();

    if (rData.mStartFrame >= 0 && rData.mStartFrame <= pFrameCtrl->getEnd()) {
        pFrameCtrl->setStart(rData.mStartFrame);
        pFrameCtrl->setLoop(rData.mStartFrame);
        pPlayer->_84 = pPlayer->_20->getFrame();
    }

    if (rData.mEndFrame >= 0 && rData.mEndFrame <= pFrameCtrl->getEnd()) {
        pFrameCtrl->setEnd(rData.mEndFrame);
    }

    if (rData.mRepeatFrame >= 0 && rData.mRepeatFrame <= pFrameCtrl->getEnd()) {
        pFrameCtrl->setLoop(rData.mRepeatFrame);
    }

    if (rData.mPlayFrame >= 0) {
        f32 speed = 0.0f;
        if (rData.mPlayFrame != 0) {
            speed = calcPlayRate(*pFrameCtrl, rData.mPlayFrame);
        }

        pPlayer->changeSpeed(speed);
    }

    if (rData.mInterpole >= 0) {
        pPlayer->changeInterpoleFrame(rData.mInterpole);
    }

    if (rData.mLoopMode != 0xFF) {
        pFrameCtrl->setAttribute(rData.mLoopMode);
    }
}
