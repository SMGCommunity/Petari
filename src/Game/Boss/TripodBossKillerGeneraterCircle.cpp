#include "Game/Boss/TripodBossKillerGeneraterCircle.hpp"
#include "Game/Boss/TripodBossKillerGenerater.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

struct GeneratorCircleData {
    const char* mName;       // 0x00
    const f32* mAngleTable;  // 0x04
    s32 mNumAngles;          // 0x08
    f32 _C;
    f32 _10;
    f32 _14;
    f32 _18;
    u32 mActiveLabel;    // 0x1C
    bool mHasCollision;  // 0x20
};

namespace {
    static f32 sUpperHorizonAngleTable[] = {42.0f, 96.0f, 140.0f, 180.0f, -140.0f, -96.0f, -42.0f};

    static f32 sUnderHorizonAngleTable[] = {78.0f, 120.0f, 160.0f, -160.0f, -120.0f, -78.0f};

    static f32 sBottomHorizonAngleTable[] = {0.0f, 120.0f, 240.0f};

    static const GeneratorCircleData sSetUpDataTable[] = {
        {"TripodBossUpperKillerCannon", sUpperHorizonAngleTable, 7, 2150.0f, 49.0f, -29.0f, 975.0f, 1, 0x1000000},
        {"TripodBossUnderKillerCannon", sUnderHorizonAngleTable, 6, 2150.0f, 5.8f, 39.2f, 975.0f, 0, 0x1000000},
        {"TripodBossBottomKillerCannon", sBottomHorizonAngleTable, 3, 500.0f, -50.0f, -30.0f, -1300.0f, 0, 0}};

    const GeneratorCircleData* getCirlceData(const char* pName) {
        for (u32 i = 0; i < 3; i++) {
            if (MR::isEqualString(pName, sSetUpDataTable[i].mName)) {
                return &sSetUpDataTable[i];
            }
        }

        return nullptr;
    }
};  // namespace

TripodBossKillerGeneraterCircle::~TripodBossKillerGeneraterCircle() {
}

TripodBossKillerGeneraterCircle::TripodBossKillerGeneraterCircle(const char* pName) : NameObj(pName) {
    mGenerators = nullptr;
    mPosition.x = 0.0f;
    mPosition.y = 0.0f;
    mPosition.z = 0.0f;
    mAngleTable = nullptr;
    mNumAngles = 0;
    _30 = 0.0f;
    _34 = 0.0f;
    _38 = 0.0f;
    _3C = 0.0f;
    _40 = 0;
}

void TripodBossKillerGeneraterCircle::init(const JMapInfoIter& rIter) {
    const char* objName;
    MR::getObjectName(&objName, rIter);
    const GeneratorCircleData* dataPtr = ::getCirlceData(objName);

    MR::getJMapInfoTrans(rIter, &mPosition);
    MR::getJMapInfoRotate(rIter, &mRotation);
    mAngleTable = dataPtr->mAngleTable;
    mNumAngles = dataPtr->mNumAngles;
    _34 = dataPtr->_C;
    _38 = dataPtr->_10;
    _3C = dataPtr->_14;
    _30 = dataPtr->_18;
    mGenerators = new TripodBossKillerGenerater[mNumAngles];

    for (s32 i = 0; i < mNumAngles; i++) {
        mGenerators[i].setActiveLebel(dataPtr->mActiveLabel);
        mGenerators[i].setHasCollision(dataPtr->mHasCollision);
        mGenerators[i].init(rIter);
    }

    placementGenerater();
}

// FIXME: reg alloc in setEulerX
// https://decomp.me/scratch/3mlGB
void TripodBossKillerGeneraterCircle::placementGenerater() {
    for (s32 i = 0; i < mNumAngles; i++) {
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
        mGenerators[i].setLocalMatrix(mtx2);
    }
}
