#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"

class JMapIdInfo;
class JMapInfoIter;

class BitFlag128 {
public:
    inline BitFlag128() {
        u32* pFlag = mFlags;

        do {
            *pFlag = 0;
            pFlag++;
        } while (pFlag < mFlags + 4);
    }

    bool get(int idx) const;
    void set(int bitIndex, bool setTrue);

    /* 0x0 */ u32 mFlags[4];
};

class ZoneSwitch : public BitFlag128 {
public:
    ZoneSwitch();
};

class SwitchIdInfo {
public:
    SwitchIdInfo(s32 switchID, const JMapInfoIter& rIter);

    s32 getSwitchNo() const;

    /* 0x0 */ JMapIdInfo* mIDInfo;
    /* 0x4 */ bool mIsGlobal;
};

class StageSwitchContainer : public NameObj {
public:
    struct ContainerSwitch {
        /* 0x0 */ s32 mZoneId;
        /* 0x4 */ ZoneSwitch* mSwitch;
    };

    StageSwitchContainer();

    virtual ~StageSwitchContainer();

    void createAndAddZone(const SwitchIdInfo& rInfo);
    ZoneSwitch* getZoneSwitch(const SwitchIdInfo& rInfo);
    ZoneSwitch* findZoneSwitchFromTable(const SwitchIdInfo& rInfo);

private:
    /* 0x0C */ MR::Vector< MR::FixedArray< ContainerSwitch, 20 > > mSwitches;
    /* 0xB0 */ ZoneSwitch* mGlobalSwitches;
};

class StageSwitchFunction {
public:
    static SwitchIdInfo* createSwitchIdInfo(const char* pSwitchName, const JMapInfoIter& rIter, bool unused);
    static void onSwitchBySwitchIdInfo(const SwitchIdInfo& rInfo);
    static void offSwitchBySwitchIdInfo(const SwitchIdInfo& rInfo);
    static bool isOnSwitchBySwitchIdInfo(const SwitchIdInfo& rInfo);
};

class StageSwitchCtrl {
public:
    StageSwitchCtrl(const JMapInfoIter& rIter);

    void onSwitchA();
    void offSwitchA();
    bool isOnSwitchA() const;
    bool isValidSwitchA() const;
    void onSwitchB();
    void offSwitchB();
    bool isOnSwitchB() const;
    bool isValidSwitchB() const;
    bool isOnSwitchAppear() const;
    bool isValidSwitchAppear() const;
    void onSwitchDead();
    void offSwitchDead();
    bool isValidSwitchDead() const;
    bool isOnAllSwitchAfterB(int idx) const;
    bool isOnAnyOneSwitchAfterB(int idx) const;

    /* 0x0 */ SwitchIdInfo* mSW_A;
    /* 0x4 */ SwitchIdInfo* mSW_B;
    /* 0x8 */ SwitchIdInfo* mSW_Appear;
    /* 0xC */ SwitchIdInfo* mSW_Dead;
};

namespace MR {
    StageSwitchCtrl* createStageSwitchCtrl(NameObj* pObj, const JMapInfoIter& rIter);
}  // namespace MR
