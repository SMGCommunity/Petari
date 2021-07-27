#pragma once

#include "JSystem/JGeometry/TVec3.h"
#include <revolution/mtx.h>

namespace JGeometry
{
    template<typename T>
    class SMatrix34C
    {
    public:
        T val[3][4];
    };

    template<class T>
    class TMatrix34 : T
    {
    public:
        void identity();
        /*{
            this->val[2][3] = 0.0f;
            this->val[1][3] = 0.0f;
            this->val[0][3] = 0.0f;
            this->val[1][2] = 0.0f;
            this->val[0][2] = 0.0f;
            this->val[2][1] = 0.0f;
            this->val[0][1] = 0.0f;
            this->val[2][0] = 0.0f;
            this->val[1][0] = 0.0f;
            this->val[2][2] = 1.0f;
            this->val[1][1] = 1.0f;
            this->val[0][0] = 1.0f;
        }*/

        void mult(const JGeometry::TVec3f &srcVec, JGeometry::TVec3f &destVec)
        {
            destVec.set<f32>
            (
                (this->val[0][3] + ((srcVec.z * this->val[0][2]) + (srcVec.x * this->val[0][0]) + srcVec.y * this->val[0][1])),
                (this->val[1][3] + ((srcVec.z * this->val[1][2]) + (srcVec.x * this->val[1][0]) + srcVec.y * this->val[1][1])),
                (this->val[2][3] + ((srcVec.z * this->val[2][2]) + (srcVec.x * this->val[2][0]) + srcVec.y * this->val[2][1]))
            );
        }

        void concat(const T &);

        operator Mtx*() { return reinterpret_cast<Mtx*>(this); }
    };

    template<class T>
    class TPosition3 : T
    {
    public:
        //inline TPosition3();

        inline void identity()
        {
            T::identity();
        }
    };
};

typedef JGeometry::TPosition3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > > TPosition3Mtxf;
