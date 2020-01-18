#include "Model/J3D/J3DPacket.h"
#include "OS/OSInterrupt.h"
#include "OS/GX/GXDisplayList.h"

void J3DDisplayListObj::callDL()
{
    GXCallDisplayList(mList, mListSize);
}

void J3DDisplayListObj::beginDL()
{
    swapBuffer();
    J3DDisplayListObj::sInterruptFlag = OSDisableInterrupts();
    GDInitGDLObj(J3DDisplayListObj::sGDLObj, mList, _C);
    __GDCurrentDL = J3DDisplayListObj::sGDLObj;
}

u32 J3DDisplayListObj::endDL()
{
    GDPadCurr32();
    OSRestoreInterrupts(J3DDisplayListObj::sInterruptFlag);
    // todo -- some logic missing here
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
    if (mChild == 0)
    {
        mChild = pPacket;
    }
    else
    {
        mParent = mChild;
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

u32 J3DMatPacket::isSame(J3DMatPacket *pPacket) const
{
    // todo
    return 0;
}

