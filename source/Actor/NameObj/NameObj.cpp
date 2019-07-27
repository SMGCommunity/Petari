#pragma once

#include "Actor/NameObj/NameObjRegister.h"
#include "MR/ObjUtil.h"
#include "SingletonHolder.h"

NameObj::NameObj(const char *name)
{
    u32 temp = 0;
    s16 temp2 = -1;

    this->mName = name;
    this->mFlags = temp;
    this->_A = temp2;

    NameObjRegister* reg = SingletonHolder<NameObjRegister>::sInstance;
    reg->add(this);
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
    JMapInfoIter tempIter;
    tempIter._0 = 0;
    tempIter._4 = -1;

    this->init(tempIter);
}

void NameObj::setName(const char* name)
{
    this->mName = name;
}

void NameObj::executeMovement()
{
    if ((this->mFlags & 0x1) == 0x1)
        return;

    this->movement();
}

void NameObj::requestSuspend()
{
    u32 flags = this->mFlags & 0x4;

    if (flags == 4)
    {
        u32 moreFlags = this->mFlags;
        moreFlags &= 0xFFFFFFFB;
        this->mFlags = moreFlags;
    }

    this->mFlags |= 0x2;
}

void NameObj::requestResume()
{
    u32 flags = this->mFlags & 0x2;

    if (flags == 0x2)
    {
        u32 moreFlags = this->mFlags;
        moreFlags &= 0xFFFFFFF9;
        this->mFlags = moreFlags;
    }

    this->mFlags |= 0x4;
}

void NameObj::syncWithFlags()
{
    u32 flags = this->mFlags & 0x2;

    if (flags == 0x2)
    {
        u32 moreFlags = this->mFlags;
        moreFlags &= 0xFFFFFFFD;
        moreFlags |= 0x1;
        this->mFlags = moreFlags;
    }

    u32 evenMoreFlags = this->mFlags;
    evenMoreFlags &= 0x4;
    
    if (evenMoreFlags != 0x4)
        return;

    evenMoreFlags = this->mFlags;
    evenMoreFlags = (evenMoreFlags & 0xFFFFFFF9) & 0xFFFE;
    this->mFlags = evenMoreFlags;
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