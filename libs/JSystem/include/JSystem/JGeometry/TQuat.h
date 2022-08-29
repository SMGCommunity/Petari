#pragma once

#include "JSystem/JGeometry/TVec.h"

namespace JGeometry {
    template<typename T>
    struct TQuat4 {
    public:
        /* Constructors */
        inline TQuat4() {}

        template<typename T>
        TQuat4(T _x, T _y, T _z, T _w) {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }

        /* General operations */
        void normalize();
        void normalize(const TQuat4<T> &rSrc);

        void getXDir(TVec3<T> &rDest) const;
        void getYDir(TVec3<T> &rDest) const;
        void getZDir(TVec3<T> &rDest) const;

        void getEuler(TVec3<T> &rDest) const;
        void setEuler(T _x, T _y, T _z);
        void setEulerZ(T _z);

        void setRotate(const TVec3<T> &, const TVec3<T> &, T);
        void setRotate(const TVec3<T> &, const TVec3<T> &);
        void setRotate(const TVec3<T> &, f32);
        void rotate(TVec3f &rDest) const;

        void slerp(const TQuat4<T> &, const TQuat4<T>, T);
        void slerp(const TQuat4<T> &, T);
        void transform(const TVec3<T> &, TVec3<T> &rDest);
        void transform(TVec3<T> &rDest);

        /* Operators */
        TQuat4<T>& operator=(const TQuat4<T> &rSrc);

        T x, y, z, w;
    };
};

typedef JGeometry::TQuat4<f32> TQuat4f;
