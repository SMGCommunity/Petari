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

// Not sure how to match the state setting.
// s32 JSURandomInputStream::seek(s32 offset, JSUStreamSeekFrom whence) {
//     s32 read = seekPos(offset, whence);
//     // mState = 1 - (mState & ~IO_ERROR);
//     mState = (mState & ~IO_ERROR) ? IO_OK : IO_ERROR;
//     return read;
// }