#include "JSystem/JSupport/JSUMemoryInputStream.hpp"
#include "JSystem/JSupport/JSUMemoryOutputStream.hpp"
#include <cstring>

void JSUMemoryInputStream::setBuffer(const void *pBuffer, s32 length) {
    mBuffer = pBuffer;
    mLength = length;
    mPosition = 0;
}

u32 JSUMemoryInputStream::readData(void *pDest, s32 length) {
    if (mPosition + length > mLength) {
        length = mLength - mPosition;
    }
    if (length > 0) {
        memcpy(pDest, (void*)((int)mBuffer + mPosition), length);
        mPosition += length;
    }
    return length;
}

s32 JSUMemoryInputStream::seekPos(s32 offset, JSUStreamSeekFrom whence) {
    s32 oldPosition = mPosition;
    switch (whence) {
        case SEEK_FROM_START:
            mPosition = offset;
            break;
        case SEEK_FROM_END:
            mPosition = mLength - offset;
            break;
        case SEEK_FROM_POSITION:
            mPosition += offset;
            break;
    }
    if (mPosition < 0) {
        mPosition = 0;
    }
    if (mPosition > mLength) {
        mPosition = mLength;
    }
    return mPosition - oldPosition;
}

void JSUMemoryOutputStream::setBuffer(void *pBuffer, s32 length) {
    mBuffer = pBuffer;
    mLength = length;
    mPosition = 0;
}

s32 JSUMemoryOutputStream::writeData(const void *pSrc, s32 length) {
    if (mPosition + length > mLength) {
        length = mLength - mPosition;
    }
    if (length > 0) {
        memcpy((void*)((int)mBuffer + mPosition), pSrc, length);
        mPosition += length;
    }
    return length;
}

s32 JSUMemoryOutputStream::seekPos(s32 offset, JSUStreamSeekFrom whence) {
    s32 oldPosition = mPosition;
    switch (whence) {
        case SEEK_FROM_START:
            mPosition = offset;
            break;
        case SEEK_FROM_END:
            mPosition = mLength - offset;
            break;
        case SEEK_FROM_POSITION:
            mPosition += offset;
            break;
    }
    if (mPosition < 0) {
        mPosition = 0;
    }
    if (mPosition > mLength) {
        mPosition = mLength;
    }
    return mPosition - oldPosition;
}

s32 JSUMemoryOutputStream::getLength() const {
    return mLength;
}

s32 JSUMemoryInputStream::getPosition() const {
    return mPosition;
}

s32 JSUMemoryInputStream::getLength() const {
    return mLength;
}