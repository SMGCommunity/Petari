#pragma once

namespace JGeometry {

    template<typename T>
    class SMatrix34C {
    public:
        T mMtx[3][4];
    };

    template<typename T>
    class TMatrix34 : public T {
    public:
        TMatrix34() {

        }

        void mult(const TVec3f &, TVec3f &);

        void mult(const TVec3f &srcVec, const TVec2f &destVec) const {
            destVec.set<f32>(   (this->mMtx[0][3]
            + ((srcVec.z * this->mMtx[0][2]) + ((srcVec.x * this->mMtx[0][0]) + (srcVec.y * this->mMtx[0][1])))),
                (this->mMtx[1][3]
            + ((srcVec.z * this->mMtx[1][2]) + ((srcVec.x * this->mMtx[1][0]) + (srcVec.y * this->mMtx[1][1])))),
                (this->mMtx[2][3]
            + ((srcVec.z * this->mMtx[2][2]) + ((srcVec.x * this->mMtx[2][0]) + (srcVec.y * this->mMtx[2][1])))));
        }

        void multTranspose(const TVec3f &a1, const TVec3f &a2) const; //{
            /*f32 v3 = this->mMtx[1][3];
            f32 v4 = this->mMtx[0][3];
            a2.set<f32>(
                (((a1.z - this->mMtx[2][3]) * this->mMtx[2][0])
            + (((a1.x - v4) * this->mMtx[0][0]) + ((a1.y - v3) * this->mMtx[1][0]))),
                (((a1.z - this->mMtx[2][3]) * this->mMtx[2][1])
            + (((a1.x - v4) * this->mMtx[0][1]) + ((a1.y - v3) * this->mMtx[1][1]))),
                (((a1.z - this->mMtx[2][3]) * this->mMtx[2][2])
            + (((a1.x - v4) * this->mMtx[0][2]) + ((a1.y - v3) * this->mMtx[1][2]))));
        }*/

        void invert(const TMatrix34<T> &);
        void concat(const T &, const T &);
        void concat(const T &);
    };
};

typedef JGeometry::SMatrix34C<f32> TSMtxf;
typedef JGeometry::TMatrix34<TSMtxf> TMtx34f;