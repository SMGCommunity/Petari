#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"

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

    u32 mNrMatrices;                         // 0xC
    MtxPtr mMatrices[NR_REGISTERS];          // 0x10
    const char* mMatrixNames[NR_REGISTERS];  // 0x110
    u32 mNrVectors;                          // 0x210
    const TVec3f* mVectors[NR_REGISTERS];    // 0x214
    const char* mVectorNames[NR_REGISTERS];  // 0x314
    u32 mNrFloats;                           // 0x414
    const f32* mFloats[NR_REGISTERS];        // 0x418
    const char* mFloatNames[NR_REGISTERS];   // 0x518
    TMtx34f mDummyMatrix;                    // 0x618
    TVec3f mDummyVector;                     // 0x648
    f32 mDummyFloat;                         // 0x654
};