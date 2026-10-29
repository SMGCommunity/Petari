#include "Game/MapObj/MapPartsRailGuidePoint.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"

namespace {
    static const f32 sShadowRadius = 20.0f;
};  //  namespace

MapPartsRailGuidePoint::MapPartsRailGuidePoint(const LiveActor* pActor, const char* pName, f32 railCoord, bool hasShadow)
    : LiveActor("レイル点"), mRailCoord(railCoord), mHasShadow(hasShadow) {
    initModelManagerWithAnm(pName, nullptr, false);
    MR::calcRailPosAtCoord(&mPosition, pActor, mRailCoord);
}

MapPartsRailGuidePoint::MapPartsRailGuidePoint(const LiveActor* pActor, const char* pName, int railPointIndex, bool hasShadow)
    : LiveActor("レイル点"), mRailCoord(), mHasShadow(hasShadow) {
    initModelManagerWithAnm(pName, nullptr, false);
    MR::calcRailPointPos(&mPosition, pActor, railPointIndex);
    mRailCoord = MR::getRailPointCoord(pActor, railPointIndex);
}

void MapPartsRailGuidePoint::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjNoMovement(this);

    if (mHasShadow) {
        MR::initShadowVolumeSphere(this, ::sShadowRadius);
        MR::setShadowDropLength(this, nullptr, 5000.0f);
        MR::onCalcShadowOneTime(this, nullptr);
    }

    kill();
}

MapPartsRailGuidePoint::~MapPartsRailGuidePoint() {
}
