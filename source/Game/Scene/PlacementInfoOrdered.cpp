#include "Game/Scene/PlacementInfoOrdered.h"

#ifndef NON_MATCHING
// functionally equiv, just isn't reloading the array size properly
PlacementInfoOrdered::PlacementInfoOrdered(int count) {
    mIndexArray = NULL;
    _4 = 0;
    mSetArray = NULL;
    mIdentiferArray = NULL;
    mCount = count;
    mIndexArray = new Index[count];
    mSetArray = new SameIdSet[count];
    mIdentiferArray = new Identifier*[count];
    MR::zeroMemory(mIdentiferArray, count);
}
#endif

/*
void PlacementInfoOrdered::requestFileLoad() {
    for (u32 i = 0; i < getUsedArrayNum(); i++) {
        Identifier* id = mIdentiferArray[i];

        if (getCreator(*id)) {
            if (id->_4 == -1) {
                
            }
        }
    }
}*/