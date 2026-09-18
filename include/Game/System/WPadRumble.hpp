#pragma once

#include <revolution/types.h>

class WPad;
struct RumblePattern;

class RumbleChannel {
public:
    void clear();
    void update();
    void setPattern(const void*, const RumblePattern&, u32, bool);

    /* 0x00 */ const RumblePattern* _0;
    /* 0x04 */ bool _4;
    /* 0x08 */ u32 _8;
    /* 0x0C */ s16 _C;
    /* 0x0E */ bool _E;
    /* 0x10 */ const void* _10;
};

class WPadRumble {
public:
    WPadRumble(WPad*);

    virtual ~WPadRumble();

    void registInstance();
    void pause();
    void stop();
    void update();
    void updateRumble();
    WPadRumble* getRumbleInstance() const;
    bool vibratePatternIfNotExist(const void*, const char*);
    bool setRumblePatternIfNotExist(const void*, const RumblePattern&, bool);
    bool findRubmlePattern(const void*, s32*, s32*, s32*, const RumblePattern&);

    /* 0x04 */ WPad* mPad;
    /* 0x08 */ bool _8;
    /* 0x0C */ u32 _C;
    /* 0x10 */ RumbleChannel mChannel[8];
    /* 0xB0 */ s32 _B0;
    /* 0xB4 */ s32 _B4;
    /* 0xB8 */ bool _B8;
    /* 0xBC */ s32 _BC;

public:
    static WPadRumble** sInstanceForCallback;
};
