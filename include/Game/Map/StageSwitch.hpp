#pragma once

#include <revolution.h>

#include "Game/NameObj/NameObj.h"
#include "Game/Util/JMapIdInfo.h"

class BitFlag128 {
public:
    inline BitFlag128() {
        mFlags[0] = 0;
        mFlags[1] = 0;
        mFlags[2] = 0;
        mFlags[3] = 0;
    }

    bool get(int) const;
    void set(int, bool);

    u32 mFlags[0x4];    // _0
};

class ZoneSwitch : public BitFlag128 {
public:
    ZoneSwitch();
};

class SwitchIdInfo {
public:
    SwitchIdInfo(s32, const JMapInfoIter &);

    s32 getSwitchNo() const;

    JMapIdInfo* mIDInfo;    // _0
    bool mIsGlobal;         // _4
};

class StageSwitchContainer : public NameObj {
public:
    struct ContainerSwitch {
        s32 mData;              // _0
        ZoneSwitch* mSwitch;    // _4
    };

    StageSwitchContainer();

    virtual ~StageSwitchContainer();

    void createAndAddZone(const SwitchIdInfo &);
    ZoneSwitch* getZoneSwitch(const SwitchIdInfo &);
    ZoneSwitch* findZoneSwitchFromTable(const SwitchIdInfo &);

    ContainerSwitch mSwitches[20];  // _C
    s32 mCount;                     // _AC
    ZoneSwitch* mGlobalSwitches;    // _B0
};

class StageSwitchFunction {
public:
    static SwitchIdInfo* createSwitchIdInfo(const char *, const JMapInfoIter &, bool);
    static void onSwitchBySwitchIdInfo(const SwitchIdInfo &);
    static void offSwitchBySwitchIdInfo(const SwitchIdInfo &);
    static bool isOnSwitchBySwitchIdInfo(const SwitchIdInfo &);
};

class StageSwitchCtrl {
public:
    StageSwitchCtrl(const JMapInfoIter &);

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
    bool isOnAllSwitchAfterB(int) const;
    bool isOnAnyOneSwitchAfterB(int) const;

    SwitchIdInfo* mSW_A;            // _0
    SwitchIdInfo* mSW_B;            // _4
    SwitchIdInfo* mSW_Appear;       // _8
    SwitchIdInfo* mSW_Dead;         // _C
};

namespace MR {
    StageSwitchCtrl* createStageSwitchCtrl(NameObj *, const JMapInfoIter &);
};