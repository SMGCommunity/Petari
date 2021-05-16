#pragma once

#include "Util/AssignableArray.h"
#include "Util/Vector.h"
#include "System/DrawBuffer/DrawBufferExecutor.h"

class DrawBufferGroup
{
public:
    DrawBufferGroup();

    u8 _0[0x18];
    //MR::Vector<MR::AssignableArray<DrawBufferExecutor *>> _0;
    //MR::Vector<MR::AssignableArray<DrawBufferExecutor *>> _C;
    s32 mDrawCameraType; // _18
    s32 _1C; // light related
    s32 _20; // ^^
};