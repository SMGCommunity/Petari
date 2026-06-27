#pragma once

#include <revolution/hbm.h>

class RSO {
public:
    static void setupRsoHomeButtonMenu();
    static void HBMCreate(const HBMDataInfo*);
    static void HBMInit(void);
    static void HBMCalc(const HBMControllerData*);
    static void HBMDraw(void);
    static HBMSelectBtnNum HBMGetSelectBtnNum(void);
    static void HBMSetAdjustFlag(int);
    static void HBMStartBlackOut(void);
};
