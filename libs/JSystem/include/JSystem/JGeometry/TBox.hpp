#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

namespace JGeometry {
    template < class T >
    struct TBox {
        TBox() : i(), f() {}
        TBox(const TBox& other) : i(other.i), f(other.f) {}

        T i, f;
    };

    template <>
    struct TBox< TVec2< f32 > > {
        f32 getWidth() const { return f.x - i.x; }
        f32 getHeight() const { return f.y - i.y; }

        bool isValid() const { return f.isAbove(i); }

        void addPos(f32 x, f32 y) { addPos(TVec2< f32 >(x, y)); }

        void addPos(const TVec2< f32 >& pos) {
            i.add(pos);
            f.add(pos);
        }

        bool intersect(const TBox< TVec2< f32 > >& other) {
            i.setMax(other.i);
            f.setMin(other.f);
            return isValid();
        }

        TVec2< f32 > i, f;
    };

    template <>
    struct TBox< TVec3< f32 > > {
        TVec3<f32> i, f;

        void extend(const TVec3f&);

        bool intersectsPoint(const TVec3f& rPos) const {
            return (rPos.x >= this->i.x && rPos.y >= this->i.y && rPos.z >= this->i.z &&
                    rPos.x <  this->f.x && rPos.y <  this->f.y && rPos.z <  this->f.z);
        }
    };

    template < typename T >
    struct TBox2 : public TBox< TVec2< T > > {
    public:
        TBox2() {}

        TBox2(const TVec2< f32 >& _i, const TVec2< f32 >& _f) {
            TBox< TVec2< T > >::i.set(_i);
            TBox< TVec2< T > >::f.set(_f);
        }

        TBox2(f32 x0, f32 y0, f32 x1, f32 y1) { set(x0, y0, x1, y1); }
        // void set<T>(const TBox2<T> &a1, const TBox2<T> &a2);

        void operator=(const JGeometry::TBox2< T >&);

        void absolute() {
            if (!this->isValid()) {
                TBox2< T > box(*this);
                this->i.setMin(box.i);
                this->i.setMin(box.f);
                this->f.setMax(box.i);
                this->f.setMax(box.f);
            }
        }

        void set(const TBox< TVec2< T > >& other) { set(other.i, other.f); }
        void set(const TVec2< f32 >& i, const TVec2< f32 >& f) { this->i.set(i), this->f.set(f); }
        void set(f32 x0, f32 y0, f32 x1, f32 y1) NO_INLINE {
            this->i.set(x0, y0);
            this->f.set(x1, y1);
        }
    };

    template < typename T >
    class TBox3 : public TBox< TVec3< T > > {
    public:
        TBox3() {}
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
typedef JGeometry::TDirBox3< f32 > TDirBox3f;
