#include "Game/NPC/MiiDatabase.hpp"
#include <JSystem/JKernel/JKRFileFinder.hpp>

MiiDatabase::~MiiDatabase() {
    delete[] _0;
    delete mStoreDataArray;
}

JKRFileFinder::~JKRFileFinder() {
}
