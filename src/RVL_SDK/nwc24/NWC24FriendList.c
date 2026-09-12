#include <revolution/nwc24.h>
#include <revolution/nwc24/NWC24Internal.h>

#define FRIEND_LIST_MAGIC FOURCC('W', 'c', 'F', 'l')
#define FRIEND_LIST_VERSION 2

static const char* FLFilePath = "/shared2/wc24/nwc24fl.bin";

static NWC24Err GetCachedFLHeader(NWC24iFLHeader** ppHeader);

NWC24Err NWC24iOpenFriendList(void) {
    NWC24iFLHeader* pHeader = (NWC24iFLHeader*)NWC24WorkP->flHeader;
    Mail_memset(pHeader, 0, sizeof(NWC24iFLHeader));
    return GetCachedFLHeader(&pHeader);
}

static NWC24Err GetCachedFLHeader(NWC24iFLHeader** ppHeader) {
    NWC24File file;
    NWC24Err result;
    NWC24Err read;
    NWC24Err close;

    *ppHeader = (NWC24iFLHeader*)NWC24WorkP->flHeader;

    if ((*ppHeader)->magic != FRIEND_LIST_MAGIC) {
        result = NWC24FOpen(&file, FLFilePath, NWC24_OPEN_NAND_R);
        if (result != NWC24_OK) {
            return result;
        }

        NWC24FSeek(&file, 0, NWC24_SEEK_BEG);
        read = NWC24FRead(*ppHeader, sizeof(NWC24iFLHeader), &file);
        close = NWC24FClose(&file);

        if (read != NWC24_OK) {
            result = read;
        } else {
            result = close;
        }

        if (result != NWC24_OK) {
            return result;
        }

        if ((*ppHeader)->magic != FRIEND_LIST_MAGIC) {
            return NWC24_ERR_BROKEN;
        }

        if ((*ppHeader)->version != FRIEND_LIST_VERSION) {
            return NWC24_ERR_VER_MISMATCH;
        }
    }

    return NWC24_OK;
}
