#include "Game/MapObj/MapPartsRailGuidePoint.hpp"
#include "Game/Util.hpp"

MapPartsRailGuidePoint::MapPartsRailGuidePoint(const LiveActor *pActor, const char *pName, f32 a3, bool a4) : LiveActor("レイル点") {
    _8C = a3;
    _90 = a4;
    initModelManagerWithAnm(pName, 0, false);
    MR::calcRailPosAtCoord(&mPosition, pActor, _8C);
}

MapPartsRailGuidePoint::MapPartsRailGuidePoint(const LiveActor *pActor, const char *pName, int a3, bool a4) : LiveActor("レイル点") {
    _90 = a4;
    _8C = 0.0f;
    initModelManagerWithAnm(pName, 0, false);
    MR::calcRailPointPos(&mPosition, pActor, a3);
    _8C = MR::getRailPointCoord(pActor, a3);
}

void MapPartsRailGuidePoint::init(const JMapInfoIter &rIter) {
    MR::connectToSceneMapObjNoMovement(this);

    if (_90) {
        MR::initShadowVolumeSphere(this, 20.0f);
        MR::setShadowDropLength(this, 0, 5000.0f);
        MR::onCalcShadowOneTime(this, 0);
    }

    kill();
}

MapPartsRailGuidePoint::~MapPartsRailGuidePoint() {
    
}