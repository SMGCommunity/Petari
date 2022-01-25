#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/NPC/TalkNodeCtrl.h"

class LiveActor;

class TalkMessageCtrl : public NameObj {
public:
    TalkMessageCtrl(LiveActor *, const TVec3f &, MtxPtr);

    virtual ~TalkMessageCtrl();

    bool inMessageArea() const;

    LiveActor* mHostActor;      // _C
    TalkNodeCtrl* mNodeCtrl;    // _10
    s32 mZoneID;                // _14
    u32 _18;
    TVec3f _1C;
    MtxPtr mMtx;                // _28
    TVec3f _2C;
    f32 _38;
    u32 _3C;
    u32 _40;
    u8 _44;
    u8 _45;
    u8 _46;
    u32 _48;
    u32 _4C;
    u32 _50;
    u32 _54;
    u32 _58;
    u32 _5C;
    u32 _60;
};