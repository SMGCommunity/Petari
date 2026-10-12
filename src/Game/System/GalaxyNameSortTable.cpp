#include "Game/System/GalaxyNameSortTable.hpp"
#include "Game/System/GalaxyInfoTable.hpp"
#include "Game/Util/JMapInfo.hpp"

s32 GalaxyNameSortTable::getGalaxySortIndex(const char* pName) {
    JMapInfo info;
    info.attach(&cGalaxyInfoTable);

    JMapInfoIter iter = info.findElement("name", pName, 0);

    if (iter == info.end()) {
        return -1;
    }

    return iter.mIndex;
}
