#pragma once

#include <revolution.h>

struct J3DTransformInfo {
    Vec mScale;        // 0x00
    S16Vec mRotation;  // 0x0C
    Vec mTranslate;    // 0x14

#ifdef __MWERKS__
    inline J3DTransformInfo& operator=(const __REGISTER J3DTransformInfo& b) {
        __REGISTER const J3DTransformInfo& var_r31 = b;
        __REGISTER J3DTransformInfo& var_r30 = *this;
        __REGISTER f32 var_f31;
        asm {
            psq_l var_f31, J3DTransformInfo.mScale(var_r31), 0, 0
            psq_st var_f31, J3DTransformInfo.mScale(var_r30), 0, 0
        }
        mScale.z = b.mScale.z;
        *(u32*)&mRotation = *(u32*)&b.mRotation;
        mRotation.z = b.mRotation.z;
        asm {
            psq_l var_f31, J3DTransformInfo.mTranslate(var_r31), 0, 0
            psq_st var_f31, J3DTransformInfo.mTranslate(var_r30), 0, 0
        }
        mTranslate.z = b.mTranslate.z;
        return *this;
    }
#endif
};

struct J3DTextureSRTInfo;

extern J3DTransformInfo const j3dDefaultTransformInfo;
extern Vec const j3dDefaultScale;
extern Mtx const j3dDefaultMtx;

void J3DCalcBBoardMtx(f32 (*)[4]);
void J3DCalcYBBoardMtx(f32 (*)[4]);
void J3DPSCalcInverseTranspose(f32 (*param_0)[4], f32 (*param_1)[3]);
void J3DGetTranslateRotateMtx(const J3DTransformInfo&, Mtx);
void J3DGetTranslateRotateMtx(s16, s16, s16, f32, f32, f32, Mtx);
void J3DGetTextureMtx(const J3DTextureSRTInfo&, const Vec&, f32 (*)[4]);
void J3DGetTextureMtxOld(const J3DTextureSRTInfo&, const Vec&, f32 (*)[4]);
void J3DGetTextureMtxMaya(const J3DTextureSRTInfo&, f32 (*)[4]);
void J3DGetTextureMtxMayaOld(const J3DTextureSRTInfo& param_0, f32 (*)[4]);
void J3DScaleNrmMtx(f32 (*)[4], const Vec&);
void J3DScaleNrmMtx33(f32 (*)[3], const Vec&);
void J3DMtxProjConcat(f32 (*)[4], f32 (*)[4], f32 (*)[4]);
void J3DPSMtxArrayConcat(f32 (*)[4], f32 (*)[4], f32 (*)[4], u32);

void J3DPSMtx33CopyFrom34(__REGISTER MtxPtr src, __REGISTER Mtx3P dst);
