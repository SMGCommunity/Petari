#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ModelObj;

class HipDropRock : public LiveActor {
public:
    /// @brief Creates a new `HipDropRock`.
    /// @param pName A pointer to the null-terminated name of the object.
    HipDropRock(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeBreak();
    void exeWreck();

    void initBreakModel();
    void initItem(const JMapInfoIter&);
    void appearItem();

    /* 0x8C */ ModelObj* mBreakModel;
    /* 0x90 */ TMtx34f mBreakModelMtx;
    /* 0xC0 */ s32 mItemType;
    /* 0xC4 */ s32 mStarPieceNum;
    /* 0xC8 */ bool mObjArg7;
};
