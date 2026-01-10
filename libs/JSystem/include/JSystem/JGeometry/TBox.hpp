#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

namespace JGeometry {
    template < class T >
    struct TBox {
        TBox() : mMin(), mMax() {}
        TBox(const TBox& other) : mMin(other.i), mMax(other.f) {}

        void extend(const TVec3f&);

        T mMin, mMax;
    };

    template < typename T >
    class TBox2 {
    public:
        TBox2() {}

        TBox2(const TVec2< f32 >& _i, const TVec2< f32 >& _f) {
            TBox< TVec2< T > >::_0.set(_i);
            TBox< TVec2< T > >::_8.set(_f);
        }

        TBox2(f32 x0, f32 y0, f32 x1, f32 y1) { set(x0, y0, x1, y1); }
        // void set<T>(const TBox2<T> &a1, const TBox2<T> &a2);

        void set(const JGeometry::TBox2< T >&, const JGeometry::TBox2< T >&);
        void operator=(const JGeometry::TBox2< T >&);

        void set(f32 x0, f32 y0, f32 x1, f32 y1) NO_INLINE {
            this->_0.set(x0, y0);
            this->_8.set(x1, y1);
        }

        void addPos(f32 x, f32 y) NO_INLINE { addPos(TVec2< f32 >(x, y)); }

        void addPos(const TVec2< f32 >& pos) NO_INLINE {
            _0.add(pos);
            _8.add(pos);
        }

        f32 getWidth() const { return _8.x - _0.x; }
        f32 getHeight() const { return _8.y - _0.y; }

        TVec2< T > _0;
        TVec2< T > _8;
    };

    template < typename T >
    class TBox3 : public TBox< TVec3< T > > {
    public:
        TBox3() {}

        bool intersectsPoint(const TVec3f&) const;
    };

    template < typename T >
    class TDirBox3 {
    public:
        JGeometry::TVec3< T > _0;
        JGeometry::TVec3< T > _C;
        JGeometry::TVec3< T > _18;
        JGeometry::TVec3< T > _24;
        JGeometry::TVec3< T > _30;
    };
};  // namespace JGeometry

typedef JGeometry::TBox2< f32 > TBox2f;
typedef JGeometry::TBox2< s16 > TBox2s;
typedef JGeometry::TBox3< f32 > TBox3f;
typedef JGeometry::TBox3< JGeometry::TVec3< f32 > > TBoxVec3f;
typedef JGeometry::TDirBox3< f32 > TDirBox3f;
