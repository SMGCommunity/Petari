#pragma once

class MarioActor;

struct Constants { 
    u8 _0[0x426]; // dummy variable
    u16 _426;
    u8 _428[0x8C];
    f32 _4B4;
    u8 _4B8[0x68];
    f32 _520;
    u8 _524[0x18];
    f32 _53C;
    u8 _540[8];
    f32 _54C;
    u16 _550;
    u16 _552;
    u16 _554;
    f32 _558
    u8 _55C[0x16C];
    u16 _6C8;
};

class MarioConst {
public:
    MarioConst();

    const Constants* _0[2];
    unsigned long _8; //luigi toggle?
};
