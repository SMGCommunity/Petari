#pragma once

#include <JSystem/JKernel/JKRarchive.hpp>
#include <revolution/rso.h>

struct HBMDataInfo {
    void* mHomeButtonArchive;         // 0x00
    void* mHomeButtonSpeakerArchive;  // 0x04
    void* mHomeButtonCsvFile;         // 0x08
    void* mHomeButtonConfigFile;      // 0x0C
    u32 _10;

    u32 mLanguage;  // 0x1C

    u32 _24;

    u32 _2C;
};

struct HBMControllerData {};

class RSO {
public:
    static void setupRsoHomeButtonMenu();
    static void HBMCreate(const HBMDataInfo*);
    static void HBMInit(void);
    static void HBMCalc(const HBMControllerData*);
    static void HBMDraw(void);
    static s32 HBMGetSelectBtnNum(void);
    static void HBMSetAdjustFlag(int);
    static void HBMStartBlackOut(void);
};
