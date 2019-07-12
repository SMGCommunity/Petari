#pragma once

#include "types.h"
#include "actor/nameobj/NameObj.h"

class NameObjGroup : public NameObj
{
public:
    NameObjGroup(const char *, s32);

    virtual ~NameObjGroup();

    void registerObj(NameObj *);
    void pauseOffAll() const;
    void initObjArray(s32);

    s32 mNumObjs; // _C
    u32 _10;
    NameObj* mObjs; // _14
};
