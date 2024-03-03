#include "Game/MapObj/EarthenPipe.hpp"

EarthenPipeMediator::EarthenPipeMediator() : NameObj("土管仲介者") {
    mNumEntries = 0;
    mPipeEntries = nullptr;
    mPipeEntries = new Entry[0x20];

    for (s32 i = 0; i < 32; i++) {
        mPipeEntries[i]._0 = nullptr;
        mPipeEntries[i]._4 = nullptr;
        mPipeEntries[i].mPipeID = -1;
    }
}

void EarthenPipeMediator::entry(EarthenPipe *pPipe, const JMapInfoIter &rIter) {
    s32 pipeID = -1;
    MR::getJMapInfoArg3NoInit(rIter, &pipeID);

    for (s32 i = 0; i < mNumEntries; i++) {
        EarthenPipeMediator::Entry* entry = &mPipeEntries[i];
        
        if (pipeID == entry->mPipeID) {
            entry->_4 = pPipe;
            entry->_0->_B0 = pPipe;
            entry->_4->_B0 = entry->_0;
            return;
        }
    }

    EarthenPipeMediator::Entry* entry = &mPipeEntries[mNumEntries];
    entry->_0 = pPipe;
    entry->mPipeID = pipeID;
    mNumEntries++;
}