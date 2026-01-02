#include "Game/Player/MarioMapCode.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/MapUtil.hpp"

FloorCode::FloorCode() {}

u32 FloorCode::getCode(const Triangle* triangle) {
    u32 code;
    if (triangle == nullptr) {
        code = -1;
    } else {
        if (!triangle->isValid()) {
            code = -1;
        } else {
            return MR::getFloorCodeIndex(triangle);
        }
    }
    return code;
}
