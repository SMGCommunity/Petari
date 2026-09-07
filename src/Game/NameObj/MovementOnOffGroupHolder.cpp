#include "Game/NameObj/MovementOnOffGroupHolder.hpp"
#include "Game/NameObj/NameObjGroup.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"

MovementOnOffGroupHolder::MovementOnOffGroupHolder(const char* pName) : NameObj(pName), mGroups() {
}

NameObjGroup* MovementOnOffGroupHolder::joinToGroup(const char* pName, NameObj* pObj, u32 numMax) {
    NameObjGroup* group = findGroupFromName(pName);

    if (group == nullptr) {
        group = createGroup(pName, numMax);
    }

    group->registerObj(pObj);

    return group;
}

void MovementOnOffGroupHolder::onMovementGroup(const char* pName) {
    NameObjGroup* group = findGroupFromName(pName);

    for (int i = 0; i < group->getObjNum(); i++) {
        MR::requestMovementOn(group->getObj(i));
    }
}

NameObjGroup* MovementOnOffGroupHolder::createGroup(const char* pName, u32 numMax) {
    NameObjGroup* group;

    group = new NameObjGroup(pName, numMax);
    group->initWithoutIter();

    mGroups.push_back(group);

    return group;
}

NameObjGroup* MovementOnOffGroupHolder::findGroupFromName(const char* pName) const {
    for (u32 i = 0; i < mGroups.size(); i++) {
        if (MR::isEqualString(mGroups[i]->getName(), pName)) {
            return mGroups[i];
        }
    }

    return nullptr;
}
