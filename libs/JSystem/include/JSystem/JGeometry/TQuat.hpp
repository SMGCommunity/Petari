#pragma once

#include "JSystem/JGeometry/TVec.hpp"

namespace JGeometry {
    template<typename T>
    struct TQuat4 : public TVec3<T> {
    public:
        /* Constructors */
        inline TQuat4() {}

        inline TQuat4(T xyz, T _w) {
            this->x = xyz;
            this->y = xyz;
            this->z = xyz;
            this->w = _w;
        }

        template<typename A>
        TQuat4(A _x, A _y, A _z, A _w) {
            this->x = _x;
            this->y = _y;
            this->z = _z;
            this->w = _w;
        }

        void set(T, T, T, T);

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

        T w;
    };
};

typedef JGeometry::TQuat4<f32> TQuat4f;
