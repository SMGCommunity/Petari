#include <revolution/fs.h>
#include <private/iosresclt.h>

#ifdef __cplusplus
extern "C" {
#endif

s32 ISFS_Read(s32 fd, u8 *pBuffer, u32 bufSize) {
    /* nullptr check and alignment to 0x20 */
    if (pBuffer == NULL || (u32)pBuffer & 31) {
        return -101;
    }

    return IOS_Read(fd, pBuffer, bufSize);
}

#ifdef __cplusplus
}
#endif