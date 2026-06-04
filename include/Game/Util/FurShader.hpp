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
        s32 getTexelOrder(u16, f32, _GXTexWrapMode) const;

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

    u32 _4;
    f32 _8;
    CShader::CIndex* mIndexArray;    // 0xC
    CShader::CLengthMap mLengthMap;  // 0x10
    f32 _1C;
    u8 _20;
    u8 _21;
    u8 _22;
    u8 _23;
    u8 _24;
    u8 _25;
};
