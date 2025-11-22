#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <cmath>

namespace JGeometry {
    template < typename T >
    struct TQuat4 : public TVec3< T > {
    public:
        /* Constructors */
        inline TQuat4() {}

        inline TQuat4(T xyz, T _w) {
            this->x = xyz;
            this->y = xyz;
            this->z = xyz;
            this->w = _w;
        }

        template < typename A >
        TQuat4(A _x, A _y, A _z, A _w) {
            this->x = _x;
            this->y = _y;
            this->z = _z;
            this->w = _w;
        }

        void set(T, T, T, T);

        /* General operations */
        void normalize();
        void normalize(const TQuat4< T >& rSrc);

        void getXDir(TVec3< T >& rDest) const {
            rDest.template set< T >(1.0f - this->y * this->y * 2.0f - this->z * this->z * 2.0f, this->x * this->y * 2.0f + this->w * this->z * 2.0f,
                                    this->x * this->z * 2.0f - this->w * this->y * 2.0f);
        }

        void getYDir(TVec3< T >& rDest) const {
            rDest.template set< T >(this->x * this->y * 2.0f - this->w * this->z * 2.0f, 1.0f - this->x * this->x * 2.0f - this->z * this->z * 2.0f,
                                    this->y * this->z * 2.0f + this->w * this->x * 2.0f);
        }

        void getZDir(TVec3< T >& rDest) const {
            rDest.template set< T >(this->x * this->z * 2.0f + this->w * this->y * 2.0f, this->y * this->z * 2.0f - this->w * this->x * 2.0f,
                                    1.0f - this->x * this->x * 2.0f - this->y * this->y * 2.0f);
        }

        void getEuler(TVec3< T >& rDest) const;
        void setEuler(T _x, T _y, T _z);
        void setEulerZ(T _z);

        void setRotate(const TVec3< T >&, const TVec3< T >&, T);
        void setRotate(const TVec3< T >&, const TVec3< T >&);

        void setRotate(const TVec3< T >& pVec, f32 pAngle) {
            f32 halfAngle = pAngle * 0.5f;
            scale(sin(halfAngle), pVec);
            this->w = cos(halfAngle);
        }

        void rotate(TVec3f& rDest) const;

        void slerp(const TQuat4< T >& a1, const TQuat4< T >& a2, T a3) {
            this->x = a1.x;
            this->y = a1.y;
            this->z = a1.z;
            this->w = a1.w;
            this->slerp(a2, a3);
        }

        void slerp(const TQuat4< T >&, T);
        void transform(const TVec3< T >&, TVec3< T >& rDest);
        void transform(TVec3< T >& rDest);

        /* Operators */
        TQuat4< T >& operator=(const TQuat4< T >& rSrc);

        T w;
    };
};  // namespace JGeometry

typedef JGeometry::TQuat4< f32 > TQuat4f;
