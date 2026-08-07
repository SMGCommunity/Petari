#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

#define NR_REGISTERS 0x40

class CameraRegisterHolder : public NameObj {
public:
    CameraRegisterHolder(const char*);

    void declareMtxReg(const char*, MtxPtr);
    MtxPtr getMtx(const char*) const;

    void declareVecReg(const char*, const TVec3f*);
    const TVec3f* getVec(const char*) const;
    void declareFloatReg(const char*, const f32*);
    const char* getDummyVecRegName() const;

    void initMtx();
    void initVec();
    void initFloat();
    void initDummyRegister();

    s32 searchName(const char* const*, u32, const char*) const;

    /* 0x00C */ u32 mNrMatrices;
    /* 0x010 */ MtxPtr mMatrices[NR_REGISTERS];
    /* 0x110 */ const char* mMatrixNames[NR_REGISTERS];
    /* 0x210 */ u32 mNrVectors;
    /* 0x214 */ const TVec3f* mVectors[NR_REGISTERS];
    /* 0x314 */ const char* mVectorNames[NR_REGISTERS];
    /* 0x414 */ u32 mNrFloats;
    /* 0x418 */ const f32* mFloats[NR_REGISTERS];
    /* 0x518 */ const char* mFloatNames[NR_REGISTERS];
    /* 0x618 */ TMtx34f mDummyMatrix;
    /* 0x648 */ TVec3f mDummyVector;
    /* 0x654 */ f32 mDummyFloat;
};
