#include "actor/nameobj/NameObjGroup.h"
#include "os/gpr.h"
#include "MR/util.h"

NameObjGroup::NameObjGroup(const char *name, s32 len) : NameObj(name)
{
    this->_C = 0;
    this->mNumObjs = 0;
    this->mObjs = 0;

    this->initObjArray(len);
}

void NameObjGroup::registerObj(NameObj *obj)
{
    *(NameObj**)(this->mObjs + this->mNumObjs) = obj;
    this->mNumObjs++;
}

void NameObjGroup::pauseOffAll() const
{
    _savegpr_29();

    u32 curObjIdx = 0;
    u32 curObjOffset = 0;

    while (curObjIdx < this->mNumObjs)
    {
        MR::requestMovementOn(this->mObjs + curObjOffset);
        curObjIdx++;
        curObjOffset += 4;
    }

    _restgpr_29();
}