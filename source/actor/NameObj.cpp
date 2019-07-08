#pragma once

#include "actor/NameObj.h"
#include "MR/util.h"
#include "SingletonHolder.h"

NameObj::NameObj(const char *name)
{
    this->mName = name;
    this->mFlags = 0;
    this->_A = -1;

    NameObjRegister* reg = SingletonHolder<NameObjRegister>::instance;
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
    if (this->mFlags & 0x1)
    {
        return;
    }

    this->movement();
}

void NameObj::requestSuspend()
{
    if (this->mFlags & 0x4 != 4)
    {
        this->mFlags |= 0x2;
        return;
    }

    this->mFlags = this->mFlags & 0xE;
    this->mFlags |= 0x2;
}

void NameObj::requestResume()
{
    if (this->mFlags & 0x2 != 2)
    {
        this->mFlags |= 0x4;
        return;
    }

    this->mFlags = this->mFlags & 0x7;
    this->mFlags |= 0x4;
}

void NameObj::syncWithFlags()
{
    u16 flags;

    if (this->mFlags & 0x2 != 0x2)
    {
        if (this->mFlags & 0x4 != 4)
            return;

        flags = this->mFlags & 0xE;
        flags = flags & 0x7FFF;
        this->mFlags = flags;
        return;
    }

    flags = this->mFlags;
    flags = flags & 0x7;
    flags |= 0x1;
    this->mFlags = flags;

    flags = this->mFlags & 0xE;
    flags = flags & 0x7FFF;
    this->mFlags = flags;
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

NameObjRegister::NameObjRegister()
{
    this->mHolder = 0;
}

void NameObjRegister::add(NameObj* obj)
{
    this->mHolder->add(obj);
}

void NameObjRegister::setCurrentHolder(NameObjHolder* holder)
{
    this->mHolder = holder;
}

void NameObjGroup::NameObjGroup(const char *name, s32 len) : NameObj(name)
{
    this->mNumObjs = 0;
    this->_10 = 0;
    this->mObjs = 0;

    this->initObjArray(len);
}

void NameObjGroup::registerObj(NameObj *obj)
{
    // TODO
}

void NameObjGroup::pauseOffAll() const
{
    s32 curIdx = 0;

    while(curIdx < this->mNumObjs)
    {
        NameObj* obj = this->mObjs[curIdx];
        MR::requestMovementOn(obj);
        curIdx++;
    }
}

void NameObjGroup::initObjArray(s32 len)
{
    this->mNumObjs = len;

    NameObj* objs = new NameObj[len];
    this->mObjs = objs;

    // TODO; finish
}