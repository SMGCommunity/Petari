#pragma once

#include <revolution/types.h>

class AutoEffectInfo;
class EffectKeeper;
class JMapInfoIter;
class LayoutActor;
class LiveActor;
class MultiSceneActor;
class MultiSceneEffectKeeper;
class PaneEffectKeeper;

class AutoEffectGroup {
public:
    AutoEffectGroup(const char* pName, int capacity);
    void add(const JMapInfoIter& rIter);

    const char* getName() const {
        return mName;
    }

    /* 0x00 */ const char* mName;
    /* 0x04 */ AutoEffectInfo** mEffects;
    /* 0x08 */ s32 mCapacity;
    /* 0x0C */ s32 mCount;
};

namespace MR {
    namespace Effect {
        void addAutoEffectsFromGroup(const AutoEffectGroup* pGroup, EffectKeeper* pKeeper, const LiveActor* pActor);
        void addAutoEffectsFromGroup(const AutoEffectGroup* pGroup, PaneEffectKeeper* pKeeper, const LayoutActor* pActor);
        void addAutoEffectsFromGroup(const AutoEffectGroup* pGroup, MultiSceneEffectKeeper* pKeeper, const MultiSceneActor* pActor);
        AutoEffectGroup* createAutoEffectGroup(const char* pName);
    }  // namespace Effect
}  // namespace MR
