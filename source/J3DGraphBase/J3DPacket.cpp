#include "J3DGraphBase/J3DPacket.h"
#include <revolution/os.h>
#include <revolution/gx.h>

#include "JKernel/JKRHeap.h"

#include "MSL_C/string.h"

u32 J3DDisplayListObj::newDisplayList(u32 a1)
{
    u32 size = (a1 + 31) & 0xFFFFFFE0;
    _C = size;

    mList = new (0x20)char[size];
    _4 = new (0x20)char[_C];

    // todo -- scheduling issue
    mListSize = 0;
    return 0;
}

u32 J3DDisplayListObj::newSingleDisplayList(u32 a1)
{
    u32 size = (a1 + 31) & 0xFFFFFFE0;
    _C = size;

    void* disp = new (0x20)char[size];
    mList = disp;
    _4 = disp;
    mListSize = 0;

    return 0;
}

u32 J3DDisplayListObj::single_To_Double()
{
    if (mList == _4)
    {
        _4 = new (0x20)char[_C];
        memcpy(_4, mList, _C);
        DCStoreRange(_4, _C);
    }

    return 0;
}

void J3DDisplayListObj::setSingleDisplayList(void *pDisp, u32 a2)
{
    mList = pDisp;
    _C = (a2 + 0x1F) & 0xFFFFFFE0;
    _4 = pDisp;
    mListSize = a2;
}

void J3DDisplayListObj::swapBuffer()
{
    void* list = mList;
    mList = _4;
    _4 = list;
}

void J3DDisplayListObj::callDL()
{
    GXCallDisplayList(mList, mListSize);
}

void J3DDisplayListObj::beginDL()
{
    swapBuffer();
    J3DDisplayListObj::sInterruptFlag = OSDisableInterrupts();
    GDInitGDLObj(&J3DDisplayListObj::sGDLObj, mList, _C);
    __GDCurrentDL = &J3DDisplayListObj::sGDLObj;
}

u32 J3DDisplayListObj::endDL()
{
    GDPadCurr32();
    OSRestoreInterrupts(J3DDisplayListObj::sInterruptFlag);
    mListSize = J3DDisplayListObj::sGDLObj.ptr - J3DDisplayListObj::sGDLObj.start;
    GDFlushCurrToMem();
    __GDCurrentDL = 0;
    return mListSize;
}

void J3DDisplayListObj::beginPatch()
{
    beginDL();
}

u32 J3DDisplayListObj::endPatch()
{
    OSRestoreInterrupts(J3DDisplayListObj::sInterruptFlag);
    __GDCurrentDL = 0;
    return mListSize;
}

u32 J3DPacket::entry(J3DDrawBuffer *pBuffer)
{
    return 1;
}

void J3DPacket::addChildPacket(J3DPacket *pPacket)
{
    if (!mChild)
    {
        mChild = pPacket;
    }
    else
    {
        pPacket->mParent = mChild;
        mChild = pPacket;
    }
}

J3DDrawPacket::J3DDrawPacket()
{
    mParent = 0;
    mChild = 0;
    _C = 0;
    _10 = 0;
    mDLObj = 0;
    _24 = 0;
}

u32 J3DDrawPacket::newDisplayList(u32 unk1)
{
    mDLObj = new J3DDisplayListObj();

    if (!mDLObj)
    {
        return 4;
    }

    u32 ret = 0;
    u32 res = mDLObj->newDisplayList(unk1);

    if (res != 0)
    {
        ret = res;
    }

    return ret;
}

u32 J3DDrawPacket::newSingleDisplayList(u32 unk1)
{
    mDLObj = new J3DDisplayListObj();

    if (!mDLObj)
    {
        return 4;
    }

    u32 ret = 0;
    u32 res = mDLObj->newSingleDisplayList(unk1);

    if (res != 0)
    {
        ret = res;
    }

    return ret;
}

void J3DDrawPacket::draw()
{
    GXCallDisplayList(mDLObj->mList, mDLObj->mListSize);
}

J3DMatPacket::J3DMatPacket() : J3DDrawPacket()
{
    _28 = 0;
    _30 = 0;
    _34 = -1;
    _38 = 0;
    _3C = 0;
}

void J3DMatPacket::addShapePacket(J3DShapePacket *pPacket)
{
    if (mShapePacket != 0)
    {
        mShapePacket = pPacket;
    }
    else
    {
        pPacket->mParent = mShapePacket;
        mShapePacket = pPacket;
    }
    
}

void J3DMatPacket::beginDiff()
{
    _28->mDLObj->beginDL();
}

void J3DMatPacket::endDiff()
{
    _28->mDLObj->endDL();
}

bool J3DMatPacket::isSame(J3DMatPacket *pPacket) const
{
    u32 thing = _34;

    bool ret = false;

    if (thing == pPacket->_34 && thing >> 31)
    {
        ret = true;
    }

    return ret;
}
