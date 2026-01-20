#pragma once

#include <revolution.h>

class J3DTexMtx;
class J3DTexGenBlock;

class J3DTexMtxObj {
public:
    Mtx& getMtx(u16 idx) { return mpTexMtx[idx]; }

    void setMtx(u16 idx, const Mtx mtx) { PSMTXCopy(mtx, mpTexMtx[idx]); }

    Mtx44& getEffectMtx(u16 idx) { return mpEffectMtx[idx]; }

    u16 getNumTexMtx() const { return mTexMtxNum; }

    /* 0x00 */ Mtx* mpTexMtx;
    /* 0x04 */ Mtx44* mpEffectMtx;
    /* 0x08 */ u16 mTexMtxNum;
};

class J3DDifferedTexMtx {
public:
    static void loadExecute(f32 const (*)[4]);

    static inline void load(const Mtx m) {
        if (sTexGenBlock != NULL)
            loadExecute(m);
    }

    static J3DTexGenBlock* sTexGenBlock;
    static J3DTexMtxObj* sTexMtxObj;
};
