#ifndef SCENEOBJHOLDER_H
#define SCENEOBJHOLDER_H

#include "Actor/NameObj/NameObj.h"

class SceneObjHolder
{
public:
    SceneObjHolder();

    NameObj* create(s32);
    bool isExist(s32) const;
    NameObj* getObj(s32) const;
    NameObj* newEachObj(s32);

    NameObj* mObjs[0x7B]; // _0
};

#endif // SCENEOBJHOLDER_H