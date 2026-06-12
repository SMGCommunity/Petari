#include "Game/NPC/MiiDatabase.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include <JSystem/JKernel/JKRFileFinder.hpp>

MiiDatabase::~MiiDatabase() {
    delete[] _0;
    delete mStoreDataArray;
}

JKRFileFinder::~JKRFileFinder() {
}
