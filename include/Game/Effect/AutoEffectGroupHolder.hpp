#pragma once

#include <revolution/types.h>

class AutoEffectGroup;
class EffectKeeper;
class LayoutActor;
class LiveActor;
class MultiSceneActor;
class MultiSceneEffectKeeper;
class PaneEffectKeeper;

class AutoEffectGroupHolder {
public:
    AutoEffectGroupHolder();
    AutoEffectGroup* find(const char* pName) const;
    bool isExist(const char* pName) const;

    /* 0x000 */ AutoEffectGroup* mGroups[0x100];
    /* 0x400 */ s32 mCount;
};

namespace MR {
    namespace Effect {
        bool createAndAddAutoEffectGroup(AutoEffectGroupHolder* pHolder, const char* pName);
        void registerAutoEffectInfos(AutoEffectGroupHolder* pHolder, EffectKeeper* pKeeper, const LiveActor* pActor, const char* pName);
        void registerAutoEffectInfos(AutoEffectGroupHolder* pHolder, PaneEffectKeeper* pKeeper, const LayoutActor* pActor, const char* pName);
        void registerAutoEffectInfos(AutoEffectGroupHolder* pHolder, MultiSceneEffectKeeper* pKeeper, const MultiSceneActor* pActor,
                                     const char* pName);
    }  // namespace Effect
}  // namespace MR
