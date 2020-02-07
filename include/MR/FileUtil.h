#ifndef FILEUTIL_H
#define FILEUTIL_H

#include "JKernel/JKRHeap.h"

namespace MR
{
    bool isFileExist(const char *, bool);

    void recieveFile(const char *);
    void mountAsyncArchive(const char *, JKRHeap *);
};

#endif // FILEUTIL_H