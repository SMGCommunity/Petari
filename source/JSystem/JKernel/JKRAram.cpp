#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRAramStream.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "smg.h"

#include <revolution/os.h>

JKRAram* JKRAram::create(u32 arg1, u32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    if (JKRAram::sAramObject == nullptr)
    {
        JKRAram::sAramObject = new JKRAram(arg1, arg2, arg5);
    }

    JKRAramStream::create(arg3);
    JKRDecomp::create(arg4);
    OSResumeThread(JKRAram::sAramObject->mThread);
    return JKRAram::sAramObject;
}