#include "Game/System/GalaxyNameSortTable.hpp"
#include "Game/Util/JMapInfo.hpp"

extern const JMapData GalaxyIDBCSV;

s32 GalaxyNameSortTable::getGalaxySortIndex(const char* pName) {
    JMapInfo info;
    info.attach(&GalaxyIDBCSV);

    JMapInfoIter iter = info.findElement("name", pName, 0);

    if (iter == info.end()) {
        return -1;
    }

    return iter.mIndex;
}
