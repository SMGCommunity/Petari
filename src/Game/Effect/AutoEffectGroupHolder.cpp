#include "Game/Effect/AutoEffectGroupHolder.hpp"
#include "Game/Effect/AutoEffectGroup.hpp"
#include "Game/Util/StringUtil.hpp"
#include <algorithm>

namespace MR {
    template < typename T >
    struct eq_ptr_case {
        eq_ptr_case(const char* pName) : mName(pName) {
        }

        bool operator()(T pValue) const {
            return MR::strcasecmp(pValue->getName(), mName) == 0;
        }

        /* 0x00 */ const char* mName;
    };
}  // namespace MR

AutoEffectGroupHolder::AutoEffectGroupHolder() : mCount() {
}

AutoEffectGroup* AutoEffectGroupHolder::find(const char* pName) const {
    AutoEffectGroup* const* pGroup = std::find_if(mGroups, mGroups + mCount, MR::eq_ptr_case< AutoEffectGroup* >(pName));

    if (pGroup != mGroups + mCount) {
        return *pGroup;
    }

    return nullptr;
}

bool AutoEffectGroupHolder::isExist(const char* pName) const {
    return std::find_if(mGroups, mGroups + mCount, MR::eq_ptr_case< AutoEffectGroup* >(pName)) != mGroups + mCount;
}

namespace MR {
    namespace Effect {
        bool createAndAddAutoEffectGroup(AutoEffectGroupHolder* pHolder, const char* pName) {
            if (pHolder->isExist(pName)) {
                return false;
            }

            AutoEffectGroup* pGroup = createAutoEffectGroup(pName);

            if (pGroup == nullptr) {
                return false;
            }

            const s32 index = pHolder->mCount++;
            pHolder->mGroups[index] = pGroup;
            return true;
        }

        void registerAutoEffectInfos(AutoEffectGroupHolder* pHolder, EffectKeeper* pKeeper, const LiveActor* pActor, const char* pName) {
            AutoEffectGroup* pGroup = pHolder->find(pName);

            if (pGroup != nullptr) {
                addAutoEffectsFromGroup(pGroup, pKeeper, pActor);
            }
        }

        void registerAutoEffectInfos(AutoEffectGroupHolder* pHolder, PaneEffectKeeper* pKeeper, const LayoutActor* pActor, const char* pName) {
            AutoEffectGroup* pGroup = pHolder->find(pName);

            if (pGroup != nullptr) {
                addAutoEffectsFromGroup(pGroup, pKeeper, pActor);
            }
        }

        void registerAutoEffectInfos(AutoEffectGroupHolder* pHolder, MultiSceneEffectKeeper* pKeeper, const MultiSceneActor* pActor,
                                     const char* pName) {
            AutoEffectGroup* pGroup = pHolder->find(pName);

            if (pGroup != nullptr) {
                addAutoEffectsFromGroup(pGroup, pKeeper, pActor);
            }
        }
    }  // namespace Effect
}  // namespace MR
