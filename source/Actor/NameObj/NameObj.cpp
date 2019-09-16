#include "Actor/NameObj/NameObjRegister.h"
#include "MR/ObjUtil.h"
#include "SingletonHolder.h"

NameObj::NameObj(const char *pName)
{
    u32 temp = 0;
    s16 temp2 = -1;

    mName = pName;
    mFlags = temp;
    _A = temp2;

    NameObjRegister* pReg = SingletonHolder<NameObjRegister>::sInstance;
    pReg->add(this);
}

void NameObj::init(const JMapInfoIter &infoIter)
{
    return;
}

void NameObj::initAfterPlacement()
{
    return;
}

void NameObj::movement()
{
    return;
}

void NameObj::draw() const
{
    return;
}

void NameObj::calcAnim()
{
    return;
}

void NameObj::calcViewAndEntry()
{
    return;
}

void NameObj::initWithoutIter()
{
    JMapInfoIter tempIter(0, -1);

    init(tempIter);
}

void NameObj::setName(const char* name)
{
    mName = name;
}

void NameObj::executeMovement()
{
    if ((mFlags & 0x1) == 0x1)
    {    
        return;
    }

    movement();
}

void NameObj::requestSuspend()
{
    u32 flags = mFlags & 0x4;

    if (flags == 4)
    {
        u32 moreFlags = mFlags;
        moreFlags &= 0xFFFFFFFB;
        mFlags = moreFlags;
    }

    mFlags |= 0x2;
}

void NameObj::requestResume()
{
    u32 flags = mFlags & 0x2;

    if (flags == 0x2)
    {
        u32 moreFlags = mFlags;
        moreFlags &= 0xFFFFFFF9;
        mFlags = moreFlags;
    }

    mFlags |= 0x4;
}

void NameObj::syncWithFlags()
{
    u32 flags = mFlags & 0x2;

    if (flags == 0x2)
    {
        u32 moreFlags = mFlags;
        moreFlags &= 0xFFFFFFFD;
        moreFlags |= 0x1;
        mFlags = moreFlags;
    }

    u32 evenMoreFlags = mFlags;
    evenMoreFlags &= 0x4;
    
    if (evenMoreFlags != 0x4)
    {
        return;
    }

    evenMoreFlags = mFlags;
    evenMoreFlags = (evenMoreFlags & 0xFFFFFFF9) & 0xFFFE;
    mFlags = evenMoreFlags;
}

void NameObjFunction::requestMovementOn(NameObj *obj)
{
    obj->requestResume();
    MR::notifyRequestNameObjMovementOnOff();
}

void NameObjFunction::requestMovementOff(NameObj *obj)
{
    obj->requestSuspend();
    MR::notifyRequestNameObjMovementOnOff();
}