#include "Game/AreaObj/WaterArea.hpp"
#include "Game/Map/WaterAreaHolder.hpp"
#include "Game/Util.hpp"

WaterArea::WaterArea(int type, const char *pName) : AreaObj(type, pName) {
    _3C = 0;
}

void WaterArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);

    if (MR::checkJMapDataEntries(rIter)) {
        MR::getJMapInfoArg2NoInit(rIter, &_3C);
    }

    WaterAreaFunction::createWaterAreaHolder();
}

WaterAreaMgr::WaterAreaMgr(s32 type, const char *pName) : AreaObjMgr(type, pName) {

}

WaterArea::~WaterArea() {

}

WaterAreaMgr::~WaterAreaMgr() {

}