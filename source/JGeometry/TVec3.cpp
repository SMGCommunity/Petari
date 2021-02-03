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

    template<typename T>
    f32 TVec3<T>::dot(const JGeometry::TVec3<T> &src) const
    {
        __asm
        {
            psq_l f2, 4(r3), 0, 0
            psq_l f1, 4(r4), 0, 0
            psq_l f0, 0(r3), 0, 0
            ps_mul f2, f2, f1
            psq_l f1, 0(r4), 0, 0
            ps_madd f1, f0, f1, f2
            ps_sum0 f1, f1, f2, f2
        }
    }

    template<typename T>
    void TVec3<T>::add(register const JGeometry::TVec3<T> &src) const
    {
        __asm
        {
            psq_l f3, 0(r3), 0, 0
            psq_l f2, 0(src), 0, 0
            psq_l f1, 8(r3), 1, 0
            psq_l f0, 8(src), 1, 0
            ps_add f2, f3, f2
            ps_add f0, f1, f0
            psq_st f2, 0(src), 0, 0
            psq_st f0, 8(src), 1, 0
        }
    }

    template<typename T>
    bool TVec3<T>::operator==(const JGeometry::TVec3<T> &rhs)
    {
        if (epsilonEquals(x, rhs.x, 0.0000038146973f))
        {
            if (epsilonEquals(y, rhs.y, 0.0000038146973f))
            {
                if (epsilonEquals(z, rhs.z, 0.0000038146973f))
                {
                    return true;
                }
            }
        }

        return false;
    }
};