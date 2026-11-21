#include "Game/System/GalaxyNameSortTable.hpp"
#include "Game/System/PowerStarNumTable.hpp"
#include "Game/Util/JMapInfo.hpp"

s32 GalaxyNameSortTable::getGalaxySortIndex(const char* pName) {
    JMapInfo info;
    info.attach(&PowerStarNumTable_bcsv_start);

    JMapInfoIter iter = info.findElement("name", pName, 0);

    if (iter == info.end()) {
        return -1;
    }

    return iter.mIndex;
}
