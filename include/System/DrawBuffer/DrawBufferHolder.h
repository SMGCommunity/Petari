#ifndef DRAWBUFFERHOLDER_H
#define DRAWBUFFERHOLDER_H

#include <revolution.h>
#include "System/DrawBuffer/DrawBufferGroup.h"

class LiveActor;

class DrawBufferHolder
{
public:
    DrawBufferHolder();

    void registerDrawBuffer(LiveActor *, s32);

    DrawBufferGroup* mGroups; // _0
};

#endif // DRAWBUFFERHOLDER_H