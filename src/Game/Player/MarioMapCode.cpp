#include "Game/Player/MarioMapCode.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/MapUtil.hpp"

FloorCode::FloorCode() {}

u32 FloorCode::getCode(const Triangle* triangle) {
    u32 res;
    if (triangle == nullptr) {
        res = -1;
    } else {
        if (!triangle->isValid()) {
            res = -1;
        } else {
            return MR::getFloorCodeIndex(triangle);
        }
    }
    return res;
}
