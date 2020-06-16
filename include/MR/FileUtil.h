#ifndef FILEUTIL_H
#define FILEUTIL_H

#include "JKernel/JKRHeap.h"

namespace MR
{
    s32 isFileExist(const char *, bool);
    u32 getFileSize(const char *, bool);
    s32 convertPathToEntrynumConsideringLanguage(const char *);

    void recieveFile(const char *);
    void mountAsyncArchive(const char *, JKRHeap *);
};

#endif // FILEUTIL_H