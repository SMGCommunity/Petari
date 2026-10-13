#include "Game/Map/StageSwitch.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/JMapIdInfo.hpp"

bool BitFlag128::get(int index) const {
    u32 flags = mFlags[index / 32];
    u32 mask = 1 << (index % 32);

    return (flags & static_cast< u32 >(mask)) == mask;
}

void BitFlag128::set(int bitIndex, bool isOn) {
    u32 byteIndex = bitIndex / 32;
    u32& flag = mFlags[byteIndex];

    if (isOn) {
        flag |= (1U << (bitIndex % 32));
    } else {
        flag &= ~(1U << (bitIndex % 32));
    }
}

ZoneSwitch::ZoneSwitch() : BitFlag128() {
    for (s32 i = 0; i < 128; i++) {
        set(i, false);
    }
}

SwitchIdInfo::SwitchIdInfo(s32 switchID, const JMapInfoIter& rIter) : mIDInfo(), mIsGlobal(switchID >= 1000) {
    mIDInfo = new JMapIdInfo(switchID, rIter);
}

s32 SwitchIdInfo::getSwitchNo() const {
    if (mIsGlobal) {
        return mIDInfo->_0 - 1000;
    }

    return mIDInfo->_0;
}

void StageSwitchContainer::createAndAddZone(const SwitchIdInfo& rInfo) {
    if (findZoneSwitchFromTable(rInfo) != nullptr) {
        return;
    }

    ZoneSwitch* pZoneSwitch = new ZoneSwitch();
    ContainerSwitch sw;
    sw.mZoneId = rInfo.mIDInfo->mZoneID;
    sw.mSwitch = pZoneSwitch;
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

        if (pContainer->mZoneId == rInfo.mIDInfo->mZoneID) {
            return pContainer->mSwitch;
        }
    }

    return nullptr;
}

namespace {
    StageSwitchContainer* getStageSwitchContainer() {
        return MR::getSceneObj< StageSwitchContainer >(SceneObj_StageSwitchContainer);
    }
};  // namespace

void StageSwitchCtrl::onSwitchA() {
    StageSwitchFunction::onSwitchBySwitchIdInfo(*mSwitchA);
}

void StageSwitchCtrl::offSwitchA() {
    StageSwitchFunction::offSwitchBySwitchIdInfo(*mSwitchA);
}

bool StageSwitchCtrl::isOnSwitchA() const {
    return StageSwitchFunction::isOnSwitchBySwitchIdInfo(*mSwitchA);
}

bool StageSwitchCtrl::isValidSwitchA() const {
    return mSwitchA != nullptr;
}

void StageSwitchCtrl::onSwitchB() {
    StageSwitchFunction::onSwitchBySwitchIdInfo(*mSwitchB);
}

void StageSwitchCtrl::offSwitchB() {
    StageSwitchFunction::offSwitchBySwitchIdInfo(*mSwitchB);
}

bool StageSwitchCtrl::isOnSwitchB() const {
    return StageSwitchFunction::isOnSwitchBySwitchIdInfo(*mSwitchB);
}

bool StageSwitchCtrl::isValidSwitchB() const {
    return mSwitchB != nullptr;
}

bool StageSwitchCtrl::isOnSwitchAppear() const {
    return StageSwitchFunction::isOnSwitchBySwitchIdInfo(*mSwitchAppear);
}

bool StageSwitchCtrl::isValidSwitchAppear() const {
    return mSwitchAppear != nullptr;
}

void StageSwitchCtrl::onSwitchDead() {
    StageSwitchFunction::onSwitchBySwitchIdInfo(*mSwitchDead);
}

void StageSwitchCtrl::offSwitchDead() {
    StageSwitchFunction::offSwitchBySwitchIdInfo(*mSwitchDead);
}

bool StageSwitchCtrl::isValidSwitchDead() const {
    return mSwitchDead != nullptr;
}

bool StageSwitchCtrl::isOnAllSwitchAfterB(int index) const {
    SwitchIdInfo* pSwitch;
    s32 switchNo;

    for (s32 i = 0; i < index; i++) {
        switchNo = i + mSwitchB->getSwitchNo();
        pSwitch = mSwitchB;

        if (!::getStageSwitchContainer()->getZoneSwitch(*pSwitch)->get(switchNo)) {
            return false;
        }
    }

    return true;
}

bool StageSwitchCtrl::isOnAnyOneSwitchAfterB(int index) const {
    SwitchIdInfo* pSwitch;
    s32 switchNo;

    for (s32 i = 0; i < index; i++) {
        switchNo = i + mSwitchB->getSwitchNo();
        pSwitch = mSwitchB;

        if (::getStageSwitchContainer()->getZoneSwitch(*pSwitch)->get(switchNo)) {
            return true;
        }
    }

    return false;
}

SwitchIdInfo* StageSwitchFunction::createSwitchIdInfo(const char* pSwitchName, const JMapInfoIter& rIter, bool) {
    s32 switchNo;
    bool hasSwitch = rIter.getValue< s32 >(pSwitchName, &switchNo);

    if (!hasSwitch) {
        return nullptr;
    }

    if (switchNo < 0) {
        return nullptr;
    }

    SwitchIdInfo* idInfo = new SwitchIdInfo(switchNo, rIter);

    if (switchNo < 1000) {
        ::getStageSwitchContainer()->createAndAddZone(*idInfo);
    }

    return idInfo;
}

void StageSwitchFunction::onSwitchBySwitchIdInfo(const SwitchIdInfo& rIdInfo) {
    s32 switchNo = rIdInfo.getSwitchNo();
    ZoneSwitch* pZoneSwitch = ::getStageSwitchContainer()->getZoneSwitch(rIdInfo);

    if (!pZoneSwitch->get(switchNo)) {
        pZoneSwitch->set(switchNo, true);
    }
}

void StageSwitchFunction::offSwitchBySwitchIdInfo(const SwitchIdInfo& rIdInfo) {
    s32 switchNo = rIdInfo.getSwitchNo();
    ZoneSwitch* pZoneSwitch = ::getStageSwitchContainer()->getZoneSwitch(rIdInfo);

    if (pZoneSwitch->get(switchNo)) {
        pZoneSwitch->set(switchNo, false);
    }
}

bool StageSwitchFunction::isOnSwitchBySwitchIdInfo(const SwitchIdInfo& rIdInfo) {
    s32 switchNo = rIdInfo.getSwitchNo();
    StageSwitchContainer* pContainer = ::getStageSwitchContainer();

    return pContainer->getZoneSwitch(rIdInfo)->get(switchNo);
}

StageSwitchCtrl* MR::createStageSwitchCtrl(NameObj*, const JMapInfoIter& rIter) {
    return new StageSwitchCtrl(rIter);
}

StageSwitchContainer::~StageSwitchContainer() {
}

StageSwitchContainer::StageSwitchContainer() : NameObj("ステージスイッチ"), mSwitches(), mGlobalSwitches() {
    mGlobalSwitches = new ZoneSwitch();
}

StageSwitchCtrl::StageSwitchCtrl(const JMapInfoIter& rIter) : mSwitchA(), mSwitchB(), mSwitchAppear(), mSwitchDead() {
    mSwitchA = StageSwitchFunction::createSwitchIdInfo("SW_A", rIter, true);
    mSwitchB = StageSwitchFunction::createSwitchIdInfo("SW_B", rIter, true);
    mSwitchAppear = StageSwitchFunction::createSwitchIdInfo("SW_APPEAR", rIter, false);
    mSwitchDead = StageSwitchFunction::createSwitchIdInfo("SW_DEAD", rIter, false);
}

bool StageSwitch_FORCE_MATCH(const JMapInfoIter& rIter, const char* pKey, s32* pValue) {
    return rIter.getValue(pKey, pValue);
}
