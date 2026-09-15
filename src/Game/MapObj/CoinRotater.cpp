#include "Game/MapObj/CoinRotater.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

CoinRotater::CoinRotater(const char* pName) : NameObj(pName), _C(), _10(), _14() {
    mRotateYMtx.identity();
    mHiSpeedRotateYMtx.identity();
    mWaterRotateMtx.identity();
    
    MR::connectToSceneMapObjMovement(this);
}

void CoinRotater::movement() {
    _C += 8.0f;
    _C = MR::repeat(_C, 0.0f, 360.0f);

    _10 += 4.0f;
    _10 = MR::repeat(_10, 0.0f, 360.0f);

    _14 += 16.0f;
    _14 = MR::repeat(_14, 0.0f, 360.0f);

    MR::makeMtxRotateY(mRotateYMtx, _C);
    MR::makeMtxRotateY(mHiSpeedRotateYMtx, _14);
    MR::makeMtxRotateY(mWaterRotateMtx, _10);
}

namespace MR {
    void createCoinRotater() {
        MR::createSceneObj(SceneObj_CoinRotater);
    }

    TMtx34f& getCoinRotateYMatrix() {
        return MR::getSceneObj< CoinRotater >(SceneObj_CoinRotater)->mRotateYMtx;
    }

    TMtx34f& getCoinHiSpeedRotateYMatrix() {
        return MR::getSceneObj< CoinRotater >(SceneObj_CoinRotater)->mHiSpeedRotateYMtx;
    }

    TMtx34f& getCoinInWaterRotateYMatrix() {
        return MR::getSceneObj< CoinRotater >(SceneObj_CoinRotater)->mWaterRotateMtx;
    }
};  // namespace MR
