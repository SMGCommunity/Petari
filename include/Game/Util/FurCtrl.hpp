#pragma once

#include "Game/NameObj/NameObj.hpp"

class FurDrawer;
class LiveActor;
class FurParam;

class FurCtrl {
public:
    FurCtrl(LiveActor*, FurParam*, bool, u8);

    void drawFur();

    LiveActor* _0;
    u32 _4;
    u32 _8;
    u8 _C;
    u8 _D;
    u8 _E;
    u8 _F;
    u32 _10;
    u16 _14;
    FurParam* _18;
    u8 _1C;
    f32 _20;
    u16 _24;
    u32 _28;
    u32 _2C;
    u32 _30;
    u32 _34;
    u32 _38;
    u32 _3C;
};

class FurDrawManager : public NameObj {
public:
    FurDrawManager(u8);

    virtual ~FurDrawManager();
    virtual void draw() const;

    void add(FurCtrl*, u8);

    u8 _C;
    u8 mNumFurCtrls;  // 0xD
    u8 _E;
    u8 _F;

    FurCtrl** _10;
    FurCtrl** _14;
    void* _18;
};
