#pragma once

#include "Game/Screen/LayoutActor.hpp"

class StarPointerGuidance : public LayoutActor {
public:
    /// @brief Creates a new `StarPointerGuidance`
    /// @param pName A pointer to the null-terminated name of the object.
    StarPointerGuidance(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();

    void checkRequest2P();
    void checkRequest1P();
    void exeAppear1P();
    void exeWait1P();
    void exeEnd1P();
    void exeAppearBlueStarGuide();
    void exeWaitBlueStarGuide();
    void exeEndBlueStarGuide();
    void exeAppear2P();
    void exeWait2P();
    void exeEnd2P();
    void exeEndWait();
    s32 exeAppearCore(Spine*, const char*, u32, const Nerve*);
    s32 exeWaitCore(Spine*, const char*, u32);
    s32 exeEndCore(Spine*, const char*, u32);
    bool request1PGuidance(const char*, bool);
    void tryResetTimeout();
    bool isExistGuidanceOrFrame();
    bool isExistGuidance();
    bool isExistFrame1P() NO_INLINE;
    bool isExistFrame2P() NO_INLINE;
    void deactive();

    /* 0x20 */ Spine* mSpineFrame1P;
    /* 0x24 */ Spine* mSpineGuidance;
    /* 0x28 */ Spine* mSpineFrame2P;
    /* 0x2C */ s32 mOutScreenTime;
    /* 0x30 */ s32 mRequestTime1P;
    /* 0x34 */ s32 mRequestTime2P;
    /* 0x38 */ const wchar_t* mGuidanceMessage;
    /* 0x3C */ const wchar_t* mPrevGuidanceMessage;
    /* 0x40 */ bool mIsActiveRequest1P;
    /* 0x41 */ bool mIsActiveRequest2P;
    /* 0x42 */ bool mIsActive;
    /* 0x43 */ bool mIsGuidanceDisabled;
    /* 0x44 */ u32 mTextLineNumGuidance;
    /* 0x48 */ u32 mTextLineNum1P;
    /* 0x4C */ u32 mTextLineNum2P;
    /* 0x50 */ bool mIsTimeOutEnabled;
};
