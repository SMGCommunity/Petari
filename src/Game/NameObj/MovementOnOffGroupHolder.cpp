#include "Game/NameObj/MovementOnOffGroupHolder.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"

MovementOnOffGroupHolder::MovementOnOffGroupHolder(const char* pName) : NameObj(pName), mGroups() {}

NameObjGroup* MovementOnOffGroupHolder::joinToGroup(const char* pName, NameObj* pobj, u32 a3) {
    NameObjGroup* pGroup = findGroupFromName(pName);

    if (pGroup == nullptr) {
        pGroup = createGroup(pName, a3);
    }

    pGroup->registerObj(pobj);

    return pGroup;
}

void MovementOnOffGroupHolder::onMovementGroup(const char* pGroupName) {
    NameObjGroup* pGroup = findGroupFromName(pGroupName);

    for (int i = 0; i < pGroup->mObjectCount; i++) {
        MR::requestMovementOn(pGroup->mObjects[i]);
    }
}

NameObjGroup* MovementOnOffGroupHolder::createGroup(const char* pGroupName, u32 count) {
    NameObjGroup* pGroup = new NameObjGroup(pGroupName, count);
    pGroup->initWithoutIter();

    mGroups.push_back(pGroup);

    return pGroup;
}

NameObjGroup* MovementOnOffGroupHolder::findGroupFromName(const char* pName) const {
    for (u32 i = 0; i < mGroups.size(); i++) {
        if (MR::isEqualString(mGroups[i]->mName, pName)) {
            return mGroups[i];
        }
    }

    return nullptr;
}

MovementOnOffGroupHolder::~MovementOnOffGroupHolder() {}
