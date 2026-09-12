#pragma once

#include <JSystem/JGeometry/TVec.hpp>

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
    void viewCalc3(u32, MtxPtr);
    void viewCalcRef(u32, J3DModel*);
    void viewCalcRefPos(u32, J3DModel*, const TVec3f&, const TVec3f&);
    bool simpleDrawSetup(J3DMaterial*);
    void simpleDrawShape(J3DMaterial*);
    void storeDisplayList(_GDLObj*, u32);
    void shapePacketDrawFast(J3DShapePacketX*) const;
    void shapeDrawFast(J3DShapeX*) const;
    void copyExtraMtxBuffer(const J3DModelX*);
    void copyAnmMtxBuffer(const J3DModelX*);
    void swapDrawBuffer(u32);
    void setDynamicDL(u8*, u32);
    void setDrawViewBuffer(MtxPtr);
    void setDrawView(u32);
    void directDraw(J3DModel*);
    void drawIn(J3DMaterial*, bool, MtxPtr, J3DModel*);

    struct Flags {
        inline void clear() {
            *(u32*)this = 0;
        }

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
        unsigned _1D : 1;
        unsigned _1E : 1;
        unsigned _1F : 1;
    };

    /* 0xDC */ u8 _DC;
    /* 0xDD */ u8 _DD;
    union {
        struct {
            /* 0xE0 */ Mtx* _E0;
            /* 0xE4 */ Mtx* _E4;
            /* 0xE8 */ Mtx* _E8;
            /* 0xEC */ Mtx* _EC;
            /* 0xF0 */ Mtx* _F0;
            /* 0xF4 */ Mtx* _F4;
            /* 0xF8 */ Mtx* _F8;
            /* 0xFC */ Mtx* _FC;
            /* 0x100 */ Mtx* _100;
            /* 0x104 */ Mtx* _104;
            /* 0x108 */ Mtx* _108;
            /* 0x10C */ Mtx* _10C;
            /* 0x110 */ Mtx* _110;
            /* 0x114 */ Mtx* _114;
            /* 0x118 */ Mtx* _118;
            /* 0x11C */ Mtx* _11C;
        };
        Mtx* mExtraMtxBuffer[16];
    };
    /* 0x120 */ void (*_120)(void*, u16);
    /* 0x124 */ void (*mShapeCallback)(J3DShape*);
    /* 0x128 */ void* _128;
    /* 0x12C */ J3DModel* _12C;
    /* 0x130 */ u8* mDisplayLists[16];
    /* 0x170 */ u32 mDisplayListSizes[16];
    /* 0x1B0 */ Flags mFlags;
    /* 0x1B4 */ u8* _1B4;
    /* 0x1B8 */ u8* _1B8;
    /* 0x1BC */ u32 _1BC;
    /* 0x1C0 */ u16 _1C0;
    /* 0x1C4 */ u8** _1C4;
    /* 0x1C8 */ u8** _1C8;
    /* 0x1CC */ u16* _1CC;
    /* 0x1D0 */ u8 _1D0;
    /* 0x1D4 */ f32 _1D4;
    /* 0x1D8 */ u32 _1D8;
    /* 0x1DC */ u32 _1DC;
    /* 0x1E0 */ s32 _1E0;
    /* 0x1E4 */ u8 _1E4;
    /* 0x1E5 */ u8 _1E5;
};
