#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class TombSpider;

class TombSpiderAcid {
public:
    TombSpiderAcid(TombSpider*);

    void start();
    void startGland();
    void end();
    void update();
    void updateEmitAcid();
    void updateDeleteAcid();
    void updateGroundMtx(TPos3f*, MtxPtr);

    /* 0x00 */ TombSpider* mParent;
    /* 0x04 */ bool mIsGlandEnabled;
    /* 0x05 */ bool mIsEmitOn;
    /* 0x06 */ bool mIsInactive;
    /* 0x08 */ s32 mEmitTimer;
    /* 0x0C */ TPos3f mSpiderEffectMtx;
    /* 0x3C */ TPos3f mGlandFrontLEffectMtx;
    /* 0x6C */ TPos3f mGlandFrontREffectMtx;
    /* 0x9C */ TPos3f mGlandRearLEffectMtx;
    /* 0xCC */ TPos3f mGlandRearREffectMtx;
};
