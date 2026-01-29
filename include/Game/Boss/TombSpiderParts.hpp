#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class TombSpider;
class TombSpiderGland;
class TombSpiderVitalSpot;
class TombSpiderAcid;
class TombSpiderThreadAttacher;

class TombSpiderParts {
public:
    TombSpiderParts(TombSpider*);

    void appearThreadAttacherAll();
    void killThreadAttacherAll();
    void pauseOffThreadAttacherAll();
    void init();
    void initParts();

    /* 0x00 */ TombSpider* mParent;
    /* 0x04 */ TombSpiderGland* mGlandFrontL;
    /* 0x08 */ TombSpiderGland* mGlandFrontR;
    /* 0x0C */ TombSpiderGland* mGlandRearL;
    /* 0x10 */ TombSpiderGland* mGlandRearR;
    /* 0x14 */ TombSpiderVitalSpot* mVitalSpotC;
    /* 0x18 */ TombSpiderVitalSpot* mVitalSpotL;
    /* 0x1C */ TombSpiderVitalSpot* mVitalSpotR;
    /* 0x20 */ TombSpiderAcid* mAcid;
    /* 0x24 */ TombSpiderThreadAttacher* mThreadAttacherWing;
    /* 0x28 */ TombSpiderThreadAttacher* mThreadAttacherFrontL;
    /* 0x2C */ TombSpiderThreadAttacher* mThreadAttacherCenterL;
    /* 0x30 */ TombSpiderThreadAttacher* mThreadAttacherRearL;
    /* 0x34 */ TombSpiderThreadAttacher* mThreadAttacherFrontR;
    /* 0x38 */ TombSpiderThreadAttacher* mThreadAttacherCenterR;
    /* 0x3C */ TombSpiderThreadAttacher* mThreadAttacherRearR;
    /* 0x40 */ MtxPtr mMtxEye;
    /* 0x44 */ MtxPtr mMtxMouth;
    /* 0x48 */ MtxPtr mMtxHip;
    /* 0x4C */ MtxPtr mMtxFrontL;
    /* 0x50 */ MtxPtr mMtxFrontR;
    /* 0x54 */ MtxPtr mMtxRearL;
    /* 0x58 */ MtxPtr mMtxRearR;
};
