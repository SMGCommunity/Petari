#include "Game/System/BinaryDataChunkHolder.hpp"
#include "Game/Util/MemoryUtil.hpp"

BinaryDataChunkHolder::BinaryDataChunkHolder(u32 bufferSize, int maxChunks)
    : mChunks(nullptr), mMaxChunks(0), mNumChunks(0), mData(nullptr), mBufferSize(bufferSize) {
    mData = new u8[mBufferSize];
    MR::zeroMemory(mData, mBufferSize);
    mChunks = new BinaryDataChunkBase*[maxChunks];
    mMaxChunks = maxChunks;
}

void BinaryDataChunkHolder::addChunk(BinaryDataChunkBase* pChunk) {
    s32 numChunks = mNumChunks;
    mNumChunks++;
    mChunks[numChunks] = pChunk;
}

u32 BinaryDataChunkHolder::makeFileBinary(u8* pData, u32 a2) {
    s32 offset = 0;

    MR::zeroMemory(pData, 4);
    pData[0] = 1;
    pData[1] = mNumChunks;

    offset += 4;
    for (s32 idx = 0; idx < mNumChunks; idx++) {
        MR::zeroMemory(mData, mBufferSize);

        BinaryDataChunkHolderChunkData* chunkData = reinterpret_cast< BinaryDataChunkHolderChunkData* >(mData);
        makeChunkData(chunkData, mBufferSize, mChunks[idx]);

        MR::copyMemory(&pData[offset], chunkData, chunkData->mChunkSize);
        offset += chunkData->mChunkSize;
    }

    return offset;
}

bool BinaryDataChunkHolder::loadFromFileBinary(const u8* pData, u32 dataSize) {
    s32 offset = 0;

    if (dataSize < calcBinarySize(pData)) {
        return false;
    }

    offset += 4;
    bool hashError = false;
    bool deserializeError = 0;

    for (s32 idx = 0; idx < pData[1]; idx++) {
        const BinaryDataChunkHolderChunkData* chunkData = reinterpret_cast< const BinaryDataChunkHolderChunkData* >(&pData[offset]);
        BinaryDataChunkBase* chunk = findFromSignature(chunkData->mSignature);

        if (chunk != nullptr) {
            if (chunkData->mHash != chunk->makeHeaderHashCode()) {
                hashError = true;
            }

            s32 status = chunk->deserialize(chunkData->getData(), chunkData->mChunkSize - chunkData->getDataOffset());
            bool deserializeSuccess;
            switch (status) {
            case 0:
            case 1:
                deserializeSuccess = true;
                break;
            default:
                deserializeSuccess = false;
                break;
            }

            deserializeError |= !deserializeSuccess;
        }
        offset += chunkData->mChunkSize;
    }

    bool success = true;
    if (hashError) {
        success = false;
    }

    if (deserializeError) {
        success = false;
    }

    if (dataSize < offset) {
        success = false;
    }

    return success;
}

void BinaryDataChunkHolder::makeChunkData(BinaryDataChunkHolderChunkData* pData, u32 bufferSize, const BinaryDataChunkBase* pChunk) {
    pData->mSignature = pChunk->getSignature();
    pData->mHash = pChunk->makeHeaderHashCode();
    pData->mChunkSize = pChunk->serialize(pData->getData(), bufferSize - pData->getDataOffset()) + pData->getDataOffset();
}

BinaryDataChunkBase* BinaryDataChunkHolder::findFromSignature(u32 signature) const {
    for (s32 idx = 0; idx < mNumChunks; idx++) {
        BinaryDataChunkBase* chunk = mChunks[idx];

        if (chunk->getSignature() == signature) {
            return chunk;
        }
    }

    return nullptr;
}

u32 BinaryDataChunkHolder::calcBinarySize(const u8* pData) {
    s32 size = 4;
    for (s32 idx = 0; idx < pData[1]; idx++) {
        size += reinterpret_cast< const BinaryDataChunkHolderChunkData* >(&pData[size])->mChunkSize;
    }
    return size;
}
