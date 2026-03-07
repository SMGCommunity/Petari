#include "revolution/nwc24/NWC24FriendList.h"
#include "revolution/nwc24/NWC24FileApi.h"
#include "revolution/nwc24/NWC24Manage.h"
#include "revolution/nwc24/NWC24StdApi.h"


#define FRIEND_LIST_MAGIC FOURCC('W', 'c', 'F', 'l')
#define FRIEND_LIST_VERSION 2

static const char* FLFilePath = "/shared2/wc24/nwc24fl.bin";

static NWC24Err GetCachedFLHeader(NWC24FLHeader** header);

NWC24Err NWC24iOpenFriendList(void) {
    NWC24FLHeader* header = (NWC24FLHeader*)NWC24WorkP->flHeader;
    Mail_memset(header, 0, sizeof(NWC24FLHeader));
    return GetCachedFLHeader(&header);
}

static NWC24Err GetCachedFLHeader(NWC24FLHeader** header) {
    NWC24File file;
    NWC24Err result;
    NWC24Err read;
    NWC24Err close;

    *header = (NWC24FLHeader*)NWC24WorkP->flHeader;

    if ((*header)->magic != FRIEND_LIST_MAGIC) {
        result = NWC24FOpen(&file, FLFilePath, NWC24_OPEN_NAND_R);
        if (result != NWC24_OK) {
            return result;
        }

        NWC24FSeek(&file, 0, NWC24_SEEK_BEG);
        read = NWC24FRead(*header, sizeof(NWC24FLHeader), &file);
        close = NWC24FClose(&file);

        if (read != NWC24_OK) {
            result = read;
        } else {
            result = close;
        }

        if (result != NWC24_OK) {
            return result;
        }

        if ((*header)->magic != FRIEND_LIST_MAGIC) {
            return NWC24_ERR_BROKEN;
        }

        if ((*header)->version != FRIEND_LIST_VERSION) {
            return NWC24_ERR_VER_MISMATCH;
        }
    }

    return NWC24_OK;
}
