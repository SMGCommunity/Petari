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
    /* 0x2C */ s32 _2C;
    /* 0x30 */ s32 _30;
    /* 0x34 */ s32 _34;
    /* 0x38 */ const wchar_t* _38;
    /* 0x3C */ const wchar_t* _3C;
    /* 0x40 */ bool _40;
    /* 0x41 */ bool _41;
    /* 0x42 */ bool _42;
    /* 0x43 */ bool _43;
    /* 0x44 */ u32 _44;
    /* 0x48 */ u32 _48;
    /* 0x4C */ u32 _4C;
    /* 0x50 */ bool _50;
};
