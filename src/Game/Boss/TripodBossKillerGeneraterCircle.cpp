#include "Game/Boss/TripodBossKillerGeneraterCircle.hpp"
#include "Game/Boss/TripodBossKillerGenerater.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

struct SetUpData {
    /* 0x00 */ const char* mName;
    /* 0x04 */ const f32* mAngleTable;
    /* 0x08 */ s32 mAngleNum;
    /* 0x0C */ f32 _C;
    /* 0x10 */ f32 _10;
    /* 0x14 */ f32 _14;
    /* 0x18 */ f32 _18;
    /* 0x1C */ u32 mActiveLabel;
    /* 0x20 */ bool mHasCollision;
};

namespace {
    static f32 sUpperHorizonAngleTable[] = {42.0f, 96.0f, 140.0f, 180.0f, -140.0f, -96.0f, -42.0f};
    static f32 sUnderHorizonAngleTable[] = {78.0f, 120.0f, 160.0f, -160.0f, -120.0f, -78.0f};
    static f32 sBottomHorizonAngleTable[] = {0.0f, 120.0f, 240.0f};
    static const SetUpData sSetUpDataTable[] = {
        {"TripodBossUpperKillerCannon", sUpperHorizonAngleTable, ARRAY_SIZE(sUpperHorizonAngleTable), 2150.0f, 49.0f, -29.0f, 975.0f, 1, 0x1000000},
        {"TripodBossUnderKillerCannon", sUnderHorizonAngleTable, ARRAY_SIZE(sUnderHorizonAngleTable), 2150.0f, 5.8f, 39.2f, 975.0f, 0, 0x1000000},
        {"TripodBossBottomKillerCannon", sBottomHorizonAngleTable, ARRAY_SIZE(sBottomHorizonAngleTable), 500.0f, -50.0f, -30.0f, -1300.0f, 0, 0}};

    const SetUpData* getSetUpData(const char* pName) {
        for (u32 i = 0; i < ARRAY_SIZE(sSetUpDataTable); i++) {
            if (MR::isEqualString(pName, sSetUpDataTable[i].mName)) {
                return &sSetUpDataTable[i];
            }
        }

        return nullptr;
    }
};  // namespace

TripodBossKillerGeneraterCircle::~TripodBossKillerGeneraterCircle() {
}

TripodBossKillerGeneraterCircle::TripodBossKillerGeneraterCircle(const char* pName)
    : NameObj(pName), mGenerater(), mPosition(0.0f, 0.0f, 0.0f), mAngleTable(), mAngleNum(), _30(), _34(), _38(), _3C(), _40() {
}

void TripodBossKillerGeneraterCircle::init(const JMapInfoIter& rIter) {
    const char* objName;
    MR::getObjectName(&objName, rIter);

    const SetUpData* dataPtr = ::getSetUpData(objName);

    MR::getJMapInfoTrans(rIter, &mPosition);
    MR::getJMapInfoRotate(rIter, &mRotation);

    mAngleTable = dataPtr->mAngleTable;
    mAngleNum = dataPtr->mAngleNum;
    _34 = dataPtr->_C;
    _38 = dataPtr->_10;
    _3C = dataPtr->_14;
    _30 = dataPtr->_18;

    mGenerater = new TripodBossKillerGenerater[mAngleNum];

    for (s32 i = 0; i < mAngleNum; i++) {
        mGenerater[i].setActiveLebel(dataPtr->mActiveLabel);
        mGenerater[i].setHasCollision(dataPtr->mHasCollision);
        mGenerater[i].init(rIter);
    }

    placementGenerater();
}

// FIXME: reg alloc in setEulerX
// https://decomp.me/scratch/3mlGB
void TripodBossKillerGeneraterCircle::placementGenerater() {
    for (s32 i = 0; i < mAngleNum; i++) {
        TPos3f mtx;
        mtx.identity();
        mtx.setEulerX(-MR::toRadian(_3C));
        mtx.setTrans(0.0f, 0.0f, _34);

        TPos3f mtx2;
        mtx2.identity();
        mtx2.setRotate(-MR::toRadian(_38), MR::toRadian(mAngleTable[i]), 0.0f);
        mtx2.concat(mtx2, mtx);
        MR::addTransMtx(mtx2, TVec3f(0.0f, _30, 0.0f));

        TPos3f trMtx;
        MR::makeMtxTR(trMtx, mPosition, mRotation);
        mtx2.concat(trMtx, mtx2);

        mGenerater[i].setLocalMatrix(mtx2);
    }
}
