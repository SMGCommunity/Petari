#include <JSystem/JGeometry/TMatrix.hpp>
#include <cmath>

inline f32 yy(f32 y) {
    return y * y;
}

template<>
void TRot3f::setRotate(const TVec3f &mLocalDirection, f32 fr1e) {
            //union {TVec3f stack_81; const Vec stack_8;};
            //stack_81.setInline(mLocalDirection);
            //const TVec3f &stack_8 = *(const TVec3f*)&bs;
            TVec3f stack_8;
            stack_8.set(mLocalDirection);
            PSVECMag(stack_8.toCVec());
            PSVECNormalize(stack_8.toCVec(), stack_8.toVec());
            //const TVec3f stack_8 = stack_8;
        
            //f32 fr1ex, fr1ey;
        
            // 8 reg -- 3 for stack_8, 3 for calculation, 1 for fr1ex, 1 for 1.0f - fr1ex
            f32 fr1ey = sin(fr1e), fr1ex = cos(fr1e);
        
            //f32 fr1ex, fr1ey;
        
            // 8 reg -- 3 for stack_8, 3 for calculation, 1 for fr1ex, 1 for 1.0f - fr1ex
            //f32 fr1ey = sin(fr1e), fr1ex = cos(fr1e);
            f32 x, y, z;
            //f32 &rx = x, &ry = y, &rz = z;
            y = stack_8.y;
            x = stack_8.x;
            z = stack_8.z;
            //f32 yy = y * y;
            //f32 (1.0f - fr1ex) = 1.0f - fr1ex;
            
            //f32 (1.0f - fr1ex) = 1.0f - fr1ex;  
            mMtx[0][0] = fr1ex + (1.0f - fr1ex) * yy(x);
            mMtx[0][1] = (1.0f - fr1ex) * x * y - fr1ey * z;
            mMtx[0][2] = (1.0f - fr1ex) * x * z + fr1ey * y;
            mMtx[1][0] = (1.0f - fr1ex) * x * y + fr1ey * z;
            mMtx[1][1] = fr1ex + (1.0f - fr1ex) * yy(y);
            mMtx[1][2] = (1.0f - fr1ex) * y * z - fr1ey * x;
            mMtx[2][0] = (1.0f - fr1ex) * x * z - fr1ey * y;
            mMtx[2][1] = (1.0f - fr1ex) * y * z + fr1ey * x;
            mMtx[2][2] = fr1ex + (1.0f - fr1ex) * yy(z);
        }

/*
template<>
void TRot3f::setRotate(const TVec3f &mLocalDirection, f32 fr1e) {
            f32 (1.0f - fr1ex);
            TVec3f stack_8;
            stack_8.set(mLocalDirection);
            PSVECMag(stack_8.toCVec());
            PSVECNormalize(stack_8.toCVec(), stack_8.toVec());
        
            //f32 fr1ex, fr1ey;
        
            // 8 reg -- 3 for stack_8, 3 for calculation, 1 for fr1ex, 1 for 1.0f - fr1ex
            f32 fr1ey = sin(fr1e), fr1ex = cos(fr1e);
            (1.0f - fr1ex) = 1.0f - fr1ex;
            f32 a00, a01, a02, a10, a11, a12, a20, a21, a22;
         
            //f32 (1.0f - fr1ex) = 1.0f - fr1ex;  
            a00 = fr1ex + (1.0f - fr1ex) * (stack_8.x * stack_8.x);
            a01 = (1.0f - fr1ex) * stack_8.x * stack_8.y - fr1ey * stack_8.z;
            a02 = (1.0f - fr1ex) * stack_8.x * stack_8.z + fr1ey * stack_8.y;
            a10 = (1.0f - fr1ex) * stack_8.x * stack_8.y + fr1ey * stack_8.z;
            a11 = fr1ex + (1.0f - fr1ex) * (stack_8.y * stack_8.y);
            a12 = (1.0f - fr1ex) * stack_8.y * stack_8.z - fr1ey * stack_8.x;
            a20 = (1.0f - fr1ex) * stack_8.x * stack_8.z - fr1ey * stack_8.y;
            a21 = (1.0f - fr1ex) * stack_8.y * stack_8.z + fr1ey * stack_8.x;
            a22 = fr1ex + (1.0f - fr1ex) * (stack_8.z * stack_8.z);

            mMtx[0][0] = a00;
            mMtx[0][1] = a01;
            mMtx[0][2] = a02;
            mMtx[1][0] = a10;
            mMtx[1][1] = a11;
            mMtx[1][2] = a12;
            mMtx[2][0] = a20;
            mMtx[2][1] = a21;
            mMtx[2][2] = a22;
        }

*/
