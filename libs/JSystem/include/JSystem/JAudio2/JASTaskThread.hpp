#pragma once

#include "JSystem/JAudio2/JASHeapCtrl.hpp"
#include "JSystem/JKernel/JKRThread.hpp"
#include <revolution/os.h>

typedef void (*JASThreadCallback)(void*);

class JASTaskThread : public JKRThread {
public:
    typedef JASMemChunkPool< 1024, JASThreadingModel::ObjectLevelLockable > ThreadMemPool;

    int sendCmdMsg(JASThreadCallback callback, void* msg);
    int sendCmdMsg(JASThreadCallback callback, const void* msg, u32 msgSize);
};
