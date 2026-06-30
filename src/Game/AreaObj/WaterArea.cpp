#include "Game/AreaObj/WaterArea.hpp"
#include "Game/Map/WaterAreaHolder.hpp"
#include "Game/Util/JMapUtil.hpp"

WaterArea::WaterArea(int formType, const char* pName) : AreaObj(formType, pName), _3C() {
}

void WaterArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);

    if (MR::checkJMapDataEntries(rIter)) {
        MR::getJMapInfoArg2NoInit(rIter, &_3C);
    }

    WaterAreaFunction::createWaterAreaHolder();
}

WaterAreaMgr::WaterAreaMgr(s32 maxNum, const char* pName) : AreaObjMgr(maxNum, pName) {
}
