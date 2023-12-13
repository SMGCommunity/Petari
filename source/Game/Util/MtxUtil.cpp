#include "Game/Util/MtxUtil.hpp"

namespace MR {
    void setMtxTrans(MtxPtr mtx, f32 x, f32 y, f32 z) {
        mtx[0][3] = x;
        mtx[1][3] = y;
        mtx[2][3] = z;
    }

    void addTransMtx(MtxPtr mtx, const TVec3f &rVec) {
        mtx[0][3] += rVec.x;
        mtx[1][3] += rVec.y;
        mtx[2][3] += rVec.z;
    }

    void addTransMtxLocal(MtxPtr mtx, const TVec3f &rVec) {
        addTransMtxLocalX(mtx, rVec.x);
        addTransMtxLocalY(mtx, rVec.y);
        addTransMtxLocalZ(mtx, rVec.z);
    }

    void addTransMtxLocalX(MtxPtr mtx, f32 x_coord) {
        mtx[0][3] = mtx[0][3] + (mtx[0][0] * x_coord);
        mtx[1][3] = mtx[1][3] + (mtx[1][0] * x_coord);
        mtx[2][3] = mtx[2][3] + (mtx[2][0] * x_coord);
    }

    void addTransMtxLocalY(MtxPtr mtx, f32 y_coord) {
        mtx[0][3] = mtx[0][3] + (mtx[0][1] * y_coord);
        mtx[1][3] = mtx[1][3] + (mtx[1][1] * y_coord);
        mtx[2][3] = mtx[2][3] + (mtx[2][1] * y_coord);
    }

    void addTransMtxLocalZ(MtxPtr mtx, f32 z_coord) {
        mtx[0][3] = mtx[0][3] + (mtx[0][2] * z_coord);
        mtx[1][3] = mtx[1][3] + (mtx[1][2] * z_coord);
        mtx[2][3] = mtx[2][3] + (mtx[2][2] * z_coord);
    }

    void extractMtxXDir(MtxPtr mtx, TVec3f *pOut) {
        pOut->x = mtx[0][0];
        pOut->y = mtx[1][0];
        pOut->z = mtx[2][0];
    }

    void extractMtxYDir(MtxPtr mtx, TVec3f *pOut) {
        pOut->x = mtx[0][1];
        pOut->y = mtx[1][1];
        pOut->z = mtx[2][1];
    }

    void extractMtxZDir(MtxPtr mtx, TVec3f *pOut) {
        pOut->x = mtx[0][2];
        pOut->y = mtx[1][2];
        pOut->z = mtx[2][2];
    }

    void extractMtxXYZDir(MtxPtr mtx, TVec3f *pOutX, TVec3f *pOutY, TVec3f *pOutZ) {
        MR::extractMtxXDir(mtx, pOutX);
        MR::extractMtxYDir(mtx, pOutY);
        MR::extractMtxZDir(mtx, pOutZ);
    }
    
    void extractMtxTrans(MtxPtr mtx, TVec3f *pOut) {
        pOut->x = mtx[0][3];
        pOut->y = mtx[1][3];
        pOut->z = mtx[2][3];
    }

    /*
    bool isSameMtx(MtxPtr lhs, MtxPtr rhs) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                if (lhs[i][j] != rhs[i][j]) {
                    return false;
                }
            }
        }

        return true;
    }
    */
};
