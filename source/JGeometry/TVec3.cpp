#include "JGeometry/TVec3.h"

namespace JGeometry
{   
    template<typename T>
    TVec3<T>::TVec3(T val)
    {
        x = val;
        y = val;
        z = val;
    }

    void negateInternal(const f32 *src, f32 *dest)
    {
        // r3 = source vector
        // r4 = destination vector
        __asm
        {
            // load X and Y from the source vector
            psq_l f1, 0(r3), 0, 0
            // load Z from the source vector
            lfs f0, 8(r3)
            // negate both X and Y
            ps_neg f1, f1
            // negate Z
            fneg f0, f0
            // store negated values X and Y into destination vector
            psq_st f1, 0(r4), 0, 0
            // store negated value Z into destination vector
            stfs f0, 8(r4)
        }
    }

    /*template<typename T>
    void TVec3<T>::set(T inX, T inY, T inZ)
    {
        x = inX;
        y = inY;
        z = inZ;
    }*/
};