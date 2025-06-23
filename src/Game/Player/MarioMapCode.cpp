#include "Game/Player/MarioMapCode.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/MapUtil.hpp"

FloorCode::FloorCode() {}

s32 FloorCode::getCode(const Triangle *pTri) {
    if (!pTri) {
        return -1;
    }
    if (pTri->isValid() == false) {
        return -1;
    }
    return MR::getFloorCodeIndex(pTri);
}
