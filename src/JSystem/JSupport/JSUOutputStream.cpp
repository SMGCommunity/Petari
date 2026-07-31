#include "JSystem/JSupport/JSUOutputStream.hpp"
#include "JSystem/JSupport/JSURandomOutputStream.hpp"

s32 JSUOutputStream::write(const void* buffer, s32 numBytes) {
    s32 bytesWrote = writeData(buffer, numBytes);
    if (bytesWrote != numBytes) {
        setState(IO_ERROR);
    }
    return bytesWrote;
}

s32 JSUOutputStream::skip(s32 count, s8 param_1) {
    s32 skipCount = 0;
    for (; skipCount < count; skipCount++) {
        if (writeData(&param_1, sizeof(param_1)) != sizeof(param_1)) {
            setState(IO_ERROR);
            break;
        }
    }

    return skipCount;
}

s32 JSURandomOutputStream::seek(s32 param_0, JSUStreamSeekFrom param_1) {
    s32 seekResult = seekPos(param_0, param_1);
    clearState(IO_ERROR);
    return seekResult;
}

s32 JSURandomOutputStream::getAvailable() const {
    return getLength() - getPosition();
}
