#include "Game/System/BinaryDataChunkHolder.hpp"

void BinaryDataChunkHolder::addChunk(BinaryDataChunkBase *pChunk) {
    s32 numChunks = mNumChunks;
    mNumChunks = numChunks + 1;
    mChunks[numChunks] = pChunk;
}

BinaryDataChunkBase* BinaryDataChunkHolder::findFromSignature(u32 signature) const {
    for (s32 i = 0; i < mNumChunks; i++) {
        BinaryDataChunkBase* chunk = mChunks[i];

        if (chunk->getSignature() == signature) {
            return chunk;
        }
    }

    return 0;
}
