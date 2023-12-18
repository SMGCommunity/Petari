#pragma once

class MarioActor;

struct Constants { 
    u8 _0[0x426]; // dummy variable
    u16 _426;
    u8 _428[0x8C];
    f32 _4B4;
    u8 _4B8[0x28];
    f32 _4E0;
    f32 _4E4;
    f32 _4E8;
    f32 _4EC;
    f32 _4F0;
    f32 _4F4;
    f32 _4F8;
    u8 _4FC[0x18];
    f32 _514;
    f32 _518;
    f32 _51C;
    f32 _520;
    f32 _524;
    u16 _528;
    f32 _52C;
    f32 _530;
    f32 _534;
    f32 _538;
    f32 _53C;
    f32 _540;
    f32 _544;
    f32 _548;
    f32 _54C;
    u16 _550;
    u16 _552;
    u16 _554;
    f32 _558;
    u8 _55C[0x5C];
    f32 _5B8;
    f32 _5BC;
    u8 _5C0[0x108];
    u16 _6C8;
};

class MarioConst {
public:
    MarioConst();

    const Constants* _0[2];
    u32 _8; //luigi toggle?
};
