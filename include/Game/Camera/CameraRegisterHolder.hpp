#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"

#define NR_REGISTERS 0x40

class CameraRegisterHolder : public NameObj {
public:
    CameraRegisterHolder(const char *);

    void declareMtxReg(const char *, MtxPtr);
    MtxPtr getMtx(const char *) const;

    void declareVecReg(const char *, const TVec3f *);
    const TVec3f *getVec(const char *) const;
    void declareFloatReg(const char *, const float *);
    const char *getDummyVecRegName() const;

    void initMtx();
    void initVec();
    void initFloat();
    void initDummyRegister();

    s32 searchName(const char *const *, unsigned long, const char *) const;

    u32 mNrMatrices;                            // _C
    MtxPtr mMatrices[NR_REGISTERS];             // _10
    const char *mMatrixNames[NR_REGISTERS];     // _110
    u32 mNrVectors;                             // _210
    const TVec3f *mVectors[NR_REGISTERS];       // _214
    const char *mVectorNames[NR_REGISTERS];     // _314
    u32 mNrFloats;                              // _414
    const float *mFloats[NR_REGISTERS];         // _418
    const char *mFloatNames[NR_REGISTERS];      // _518
    TMtx34f mDummyMatrix;                       // _618
    TVec3f mDummyVector;                        // _648
    f32 mDummyFloat;                            // _654
};