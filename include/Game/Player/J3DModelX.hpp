#pragma once

#include <JSystem/J3DGraphAnimator/J3DModel.hpp>
#include <revolution/gd/GDBase.h>

class J3DShape;
class J3DShapeX;
class J3DShapePacketX;

class J3DModelX : public J3DModel {
public:
    J3DModelX(J3DModelData*, u32, u32);

    virtual ~J3DModelX();

    void viewCalc2();
    void simpleDrawShape(J3DMaterial*);
    void storeDisplayList(_GDLObj*, u32);
    void shapePacketDrawFast(J3DShapePacketX*);
    void shapeDrawFast(J3DShapeX*) const;
    void setDynamicDL(u8*, u32);
    void setDrawView(u32);
    void directDraw(J3DModel*);

    struct Flags {
        inline void clear() { *(u32*)this = 0; }

        unsigned _0 : 1;
        unsigned _1 : 1;
        unsigned _2 : 1;
        unsigned _3 : 1;
        unsigned _4 : 1;
        unsigned _5 : 1;
        unsigned _6 : 1;
        unsigned _7 : 1;
        unsigned _8 : 1;
        unsigned _9 : 1;
        unsigned _A : 1;
        unsigned _B : 1;
        unsigned _C : 1;
        unsigned _D : 1;
        unsigned _E : 1;
        unsigned _F : 1;
        unsigned _10 : 1;
        unsigned _11 : 1;
        unsigned _12 : 1;
        unsigned _13 : 1;
        unsigned _14 : 1;
        unsigned _15 : 1;
        unsigned _16 : 1;
        unsigned _17 : 1;
        unsigned _18 : 1;
        unsigned _19 : 1;
        unsigned _1A : 1;
        unsigned _1B : 1;
        unsigned _1C : 1;
    };

    u8 _DC;
    u8 _DD;
    u32 _E0;
    u32 _E4;
    u32 _E8;
    u32 _EC;
    u32 _F0;
    u32 _F4;
    u32 _F8;
    u32 _FC;
    u32 _100;
    u32 _104;
    u32 _108;
    u32 _10C;
    u32 _110;
    u32 _114;
    u32 _118;
    u32 _11C;
    u32 _120;
    void (*mShapeCallback)(J3DShape*);
    u32 _128;
    u32 _12C;
    u8 _130[0x1B0 - 0x130];
    Flags mFlags;
    u32 _1B4;
    u32 _1B8;
    u32 _1BC;
    u32 _1C0;
    u32* _1C4;
    u32* _1C8;
    u16* _1CC;
    u8 _1D0;
    f32 _1D4;
    u32 _1D8;
    u32 _1DC;
    s32 _1E0;
    u8 _1E4;
    u8 _1E5;
};
