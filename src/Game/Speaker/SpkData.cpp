#include "Game/Speaker/SpkData.hpp"

SpkData::SpkData(JKRArchive * archive) {
    mArchive = archive;
}

void SpkData::loadTable(u16 table) {
    void* resource = mArchive->getResource(table);
    mTable.setResource(resource);
}

void SpkData::loadWave(u16 wave) {
    void* resource = mArchive->getResource(wave);
    mWave.setResource(resource);
}

bool SpkData::isValid() const {
    return mTable.mInitialized && mWave.mResource;
}
