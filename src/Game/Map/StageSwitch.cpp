#include "Game/Map/StageSwitch.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"

bool BitFlag128::get(int idx) const {
    u32 flags = mFlags[idx / 32];
    u32 mask = (1 << (idx % 32));
    return mask == (flags & (u32)mask);
}

void BitFlag128::set(int bit_index, bool setTrue) {
    u32 valIdx = bit_index / 32;
    u32& flag = mFlags[valIdx];

    if (setTrue) {
        flag |= (1 << (bit_index % 32));
    } else {
        flag &= ~(1 << (bit_index % 32));
    }
}

ZoneSwitch::ZoneSwitch() : BitFlag128() {
    s32 idx = 0;

    while (idx < 0x80) {
        set(idx, false);
    }
}

SwitchIdInfo::SwitchIdInfo(s32 switchID, const JMapInfoIter& rIter) {
    mIDInfo = nullptr;
    mIsGlobal = switchID >= 1000;
    mIDInfo = new JMapIdInfo(switchID, rIter);
}

s32 SwitchIdInfo::getSwitchNo() const {
    if (mIsGlobal) {
        return mIDInfo->_0 - 1000;
    }

    return mIDInfo->_0;
}

void StageSwitchContainer::createAndAddZone(const SwitchIdInfo& rInfo) {
    if (findZoneSwitchFromTable(rInfo)) {
        return;
    }

    ZoneSwitch* s = new ZoneSwitch();
    ContainerSwitch sw;
    sw.mData = rInfo.mIDInfo->mZoneID;
    sw.mSwitch = s;
    mSwitches.push_back(sw);
}

ZoneSwitch* StageSwitchContainer::getZoneSwitch(const SwitchIdInfo& rInfo) {
    if (rInfo.mIsGlobal) {
        return mGlobalSwitches;
    }

    return findZoneSwitchFromTable(rInfo);
}

ZoneSwitch* StageSwitchContainer::findZoneSwitchFromTable(const SwitchIdInfo& rInfo) {
    for (int i = 0; i < mSwitches.size(); i++) {
        ContainerSwitch* pContainer = &mSwitches[i];

        if (pContainer->mData == rInfo.mIDInfo->mZoneID) {
            return pContainer->mSwitch;
        }
    }

    return nullptr;
}

void StageSwitchCtrl::onSwitchA() {
    StageSwitchFunction::onSwitchBySwitchIdInfo(*mSW_A);
}

void StageSwitchCtrl::offSwitchA() {
    StageSwitchFunction::offSwitchBySwitchIdInfo(*mSW_A);
}

bool StageSwitchCtrl::isOnSwitchA() const {
    return StageSwitchFunction::isOnSwitchBySwitchIdInfo(*mSW_A);
}

bool StageSwitchCtrl::isValidSwitchA() const {
    return mSW_A != 0;
}

void StageSwitchCtrl::onSwitchB() {
    StageSwitchFunction::onSwitchBySwitchIdInfo(*mSW_B);
}

void StageSwitchCtrl::offSwitchB() {
    StageSwitchFunction::offSwitchBySwitchIdInfo(*mSW_B);
}

bool StageSwitchCtrl::isOnSwitchB() const {
    return StageSwitchFunction::isOnSwitchBySwitchIdInfo(*mSW_B);
}

bool StageSwitchCtrl::isValidSwitchB() const {
    return mSW_B != 0;
}

bool StageSwitchCtrl::isOnSwitchAppear() const {
    return StageSwitchFunction::isOnSwitchBySwitchIdInfo(*mSW_Appear);
}

bool StageSwitchCtrl::isValidSwitchAppear() const {
    return mSW_Appear != 0;
}

void StageSwitchCtrl::onSwitchDead() {
    StageSwitchFunction::onSwitchBySwitchIdInfo(*mSW_Dead);
}

void StageSwitchCtrl::offSwitchDead() {
    StageSwitchFunction::offSwitchBySwitchIdInfo(*mSW_Dead);
}

bool StageSwitchCtrl::isValidSwitchDead() const {
    return mSW_Dead != 0;
}

#ifdef NON_MATCHING
// reg use
bool StageSwitchCtrl::isOnAllSwitchAfterB(int idx) const {
    for (s32 i = 0; i < idx; i++) {
        s32 switchNo = i + mSW_B->getSwitchNo();
        StageSwitchContainer* pContainer = MR::getSceneObj< StageSwitchContainer >(SceneObj_StageSwitchContainer);

        if (!pContainer->getZoneSwitch(*mSW_B)->get(switchNo)) {
            return false;
        }
    }

    return true;
}
#endif

// isOnAnyOneSwitchAfterB

SwitchIdInfo* StageSwitchFunction::createSwitchIdInfo(const char* pSwitchName, const JMapInfoIter& rIter, bool unused) {
    s32 switchNo;
    bool res = rIter.getValue< s32 >(pSwitchName, &switchNo);

    if (!res) {
        return nullptr;
    }

    if (switchNo < 0) {
        return nullptr;
    }

    SwitchIdInfo* inf = new SwitchIdInfo(switchNo, rIter);

    if (switchNo < 0x3E8) {
        StageSwitchContainer* pContainer = MR::getSceneObj< StageSwitchContainer >(SceneObj_StageSwitchContainer);

        pContainer->createAndAddZone(*inf);
    }

    return inf;
}

void StageSwitchFunction::onSwitchBySwitchIdInfo(const SwitchIdInfo& rSwitchId) {
    s32 switchNo = rSwitchId.getSwitchNo();
    StageSwitchContainer* pContainer = MR::getSceneObj< StageSwitchContainer >(SceneObj_StageSwitchContainer);

    ZoneSwitch* pZoneSwitch = pContainer->getZoneSwitch(rSwitchId);

    if (!pZoneSwitch->get(switchNo)) {
        pZoneSwitch->set(switchNo, true);
    }
}

void StageSwitchFunction::offSwitchBySwitchIdInfo(const SwitchIdInfo& rSwitchId) {
    s32 switchNo = rSwitchId.getSwitchNo();
    StageSwitchContainer* pContainer = MR::getSceneObj< StageSwitchContainer >(SceneObj_StageSwitchContainer);

    ZoneSwitch* pZoneSwitch = pContainer->getZoneSwitch(rSwitchId);

    if (pZoneSwitch->get(switchNo)) {
        pZoneSwitch->set(switchNo, false);
    }
}

bool StageSwitchFunction::isOnSwitchBySwitchIdInfo(const SwitchIdInfo& rSwitchId) {
    s32 switchNo = rSwitchId.getSwitchNo();
    StageSwitchContainer* pContainer = MR::getSceneObj< StageSwitchContainer >(SceneObj_StageSwitchContainer);

    ZoneSwitch* pZoneSwitch = pContainer->getZoneSwitch(rSwitchId);

    return pZoneSwitch->get(switchNo);
}

namespace MR {
    StageSwitchCtrl* createStageSwitchCtrl(NameObj* pObj, const JMapInfoIter& rIter) {
        return new StageSwitchCtrl(rIter);
    }
};  // namespace MR

StageSwitchContainer::~StageSwitchContainer() {}

StageSwitchContainer::StageSwitchContainer() : NameObj("ステージスイッチ"), mSwitches(), mGlobalSwitches() {
    mGlobalSwitches = new ZoneSwitch();
}

StageSwitchCtrl::StageSwitchCtrl(const JMapInfoIter& rIter) : mSW_A(nullptr), mSW_B(nullptr), mSW_Appear(nullptr), mSW_Dead(nullptr) {
    mSW_A = StageSwitchFunction::createSwitchIdInfo("SW_A", rIter, true);
    mSW_B = StageSwitchFunction::createSwitchIdInfo("SW_B", rIter, true);
    mSW_Appear = StageSwitchFunction::createSwitchIdInfo("SW_APPEAR", rIter, false);
    mSW_Dead = StageSwitchFunction::createSwitchIdInfo("SW_DEAD", rIter, false);
}
