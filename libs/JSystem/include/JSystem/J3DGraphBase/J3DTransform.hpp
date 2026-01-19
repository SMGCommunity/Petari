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
