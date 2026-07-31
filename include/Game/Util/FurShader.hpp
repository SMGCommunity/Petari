#pragma once

#include "revolution/gx/GXEnum.h"
#include <revolution.h>

class J3DModel;
class J3DModelData;
class ResTIMG;
class J3DShape;

class CShader {
public:
    class CLengthMap {
    public:
        CLengthMap(const ResTIMG*);

        void setLengthMap(const ResTIMG*);
        void refer(f32, f32) const;
        s32 getTexelOrder(u16, f32, GXTexWrapMode) const;

        const ResTIMG* _0;
        u32 _4;
        u8 _8;
    };

    class CIndex {
    public:
        CIndex();

        u16 _0;
        u16 _2;
    };

    CShader(const J3DModelData*, const ResTIMG*);

    virtual void calc(J3DModel*);
    virtual void setup(J3DModelData*);
    virtual ~CShader();

    void makeIndexData(J3DShape*) const;
    void checkBorderVtx(J3DModelData*, u32);

    /* 0x04 */ u32 _4;
    /* 0x08 */ f32 _8;
    /* 0x0C */ CShader::CIndex* mIndexArray;
    /* 0x10 */ CShader::CLengthMap mLengthMap;
    /* 0x1C */ f32 _1C;
    /* 0x20 */ u8 _20;
    /* 0x21 */ u8 _21;
    /* 0x22 */ u8 _22;
    /* 0x23 */ u8 _23;
    /* 0x24 */ u8 _24;
    /* 0x25 */ u8 _25;
};
