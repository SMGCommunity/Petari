#include "Game/MapObj/CoinRotater.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

CoinRotater::CoinRotater(const char *pName) : NameObj(pName) {
    _C = 0.0f;
    _10 = 0.0f;
    _14 = 0.0f;
    mRotateYMtx.identity();
    mHiSpeedRotateYMtx.identity();
    mWaterRotateMtx.identity();
    MR::connectToSceneMapObjMovement(this);
}
 
#ifndef NON_MATCHING
// regalloc
void CoinRotater::movement() {
    f32 v2 = MR::subtractFromSum(8.0f, _C, 0.0f);
    _C += 8.0f;
    f32 v3 = MR::modAndAdd(0.0f, v2, 360.0f);
    f32 v4 = MR::subtractFromSum(0.0f, 4.0f, _10);
    _10 += + 4.0f;
    _C = v3;
    f32 v5 = MR::modAndAdd(0.0f, v4, 360.0f);
    f32 v6 = MR::subtractFromSum(_14, 16.0f, 0.0f);
    _14 += _14 + 16.0f;
    _10 = v5;
    f32 v7 = fmod(360.0f + v6, 360.0f);
    _14 = MR::add(0.0f, v7);
    MR::makeMtxRotateY(mRotateYMtx.mMtx, _C);
    MR::makeMtxRotateY(mHiSpeedRotateYMtx.mMtx, _14);
    MR::makeMtxRotateY(mWaterRotateMtx.mMtx, _10);
}
#endif

namespace MR {
    void createCoinRotater() {
        MR::createSceneObj(SceneObj_CoinRotater);
    }

    TMtx34f& getCoinRotateYMatrix() {
        return MR::getSceneObj<CoinRotater*>(SceneObj_CoinRotater)->mRotateYMtx;
    }

    TMtx34f& getCoinHiSpeedRotateYMatrix() {
        return MR::getSceneObj<CoinRotater*>(SceneObj_CoinRotater)->mHiSpeedRotateYMtx;
    }

    TMtx34f& getCoinInWaterRotateYMatrix() {
        return MR::getSceneObj<CoinRotater*>(SceneObj_CoinRotater)->mWaterRotateMtx;
    }
};

CoinRotater::~CoinRotater() {

}
