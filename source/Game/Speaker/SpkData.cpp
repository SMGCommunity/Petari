#include "Game/Speaker/SpkData.h"

SpkData::SpkData(JKRArchive * archive) {
    mArchive = archive;
}

void SpkData::loadTable(u16 table) {
    void* resource = FUN_8040f918(mArchive, table);
    mTable.setResource(resource);
}

void SpkData::loadWave(u16 wave) {
    void* resource = FUN_8040f918(mArchive, wave);
    mWave.setResource(resource);
}

bool SpkData::isValid() const {
    return mTable.mInitialized && mWave.mResource;
}