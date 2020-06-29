#include "Scene/SceneObjHolder.h"
#include "Actor/NameObj/NameObjGroup.h"
#include "Actor/NameObj/NameObjExecuteHolder.h"
#include "Actor/LiveActor/AllLiveActorGroup.h"

SceneObjHolder::SceneObjHolder()
{
    s32 curIdx = 0;

    while(curIdx != 0x7B)
    {
        mObjs[curIdx++] = NULL;
    }
}

NameObj* SceneObjHolder::create(s32 type)
{
    if (mObjs[type] != 0)
    {
        return mObjs[type];
    }

    NameObj* obj = newEachObj(type);
    obj->initWithoutIter();
    mObjs[type] = obj;
    return obj;
}

NameObj* SceneObjHolder::getObj(s32 type) const
{
    return mObjs[type];
}

bool SceneObjHolder::isExist(s32 type) const
{
    return mObjs[type];
}

NameObj* SceneObjHolder::newEachObj(s32 type)
{
    NameObj* obj;

    switch (type)
    {
        case 24:
            obj = new NameObjGroup("IgnorePauseNameObj", 0x10);
            break;
        case 48:
            obj = new AllLiveActorGroup();
            break;
        default:
            obj = 0;
            break;
    }

    return obj;
}