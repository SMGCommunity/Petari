#include "JSystem/JSupport/JSURandomInputStream.h"

s32 JSUInputStream::read(void *pDest, s32 length) {
    s32 read = readData(pDest, length);
    if (read != length) {
        setState(IO_ERROR);
    }
    return read;
}

s32 JSURandomInputStream::skip(s32 amount) {
    s32 read = seekPos(amount, SEEK_FROM_POSITION);
    if (read != amount) {
        setState(IO_ERROR);
    }
    return read;
}

#ifdef NON_MATCHING
s32 JSURandomInputStream::seek(s32 offset, JSUStreamSeekFrom whence) {
    s32 read = seekPos(offset, whence);
    mState = (mState & 0x1) != 0;
    return read;
}
#endif
