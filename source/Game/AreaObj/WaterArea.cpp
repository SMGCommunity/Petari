#include "Game/AreaObj/WaterArea.h"
#include "Game/Map/WaterAreaHolder.h"

WaterArea::WaterArea(int type, const char *pName) : AreaObj(type, pName) {
    _3C = 0;
}

#ifdef NON_MATCHING
// Register mismatch
void WaterArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);

    bool bVar3 = false;
    bool bVar2 = false;

    if (rIter.mInfo != NULL && rIter._4 >= 0) {
        bVar2 = true;
    }

    if (bVar2) {
        s32 iVar1;

        if (rIter.mInfo->mData != NULL) {
            iVar1 = rIter.mInfo->mData->_0;
        }
        else {
            iVar1 = 0;
        }

        if (rIter._4 < iVar1) {
            bVar3 = true;
        }
    }

    if (bVar3) {
        MR::getJMapInfoArg2NoInit(rIter, &_3C);
    }

    WaterAreaFunction::createWaterAreaHolder();
}
#endif

WaterAreaMgr::WaterAreaMgr(s32 type, const char *pName) : AreaObjMgr(type, pName) {

}

WaterArea::~WaterArea() {

}

WaterAreaMgr::~WaterAreaMgr() {

}