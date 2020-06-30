#ifndef FILELOADERTHREAD_H
#define FILELOADERTHREAD_H

#include "System/OS/OSThreadWrapper.h"

class JKRHeap;

class FileLoaderThread : public OSThreadWrapper
{
public:
    FileLoaderThread(s32, s32, JKRHeap *);

    virtual ~FileLoaderThread();
    virtual s32 run();
};

#endif // FILELOADERTHREAD_H