#pragma once

#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/Array.hpp"

class GalaxyMoveArgument;
class NameObj;

class DemoFortressDiscoverCheckList {
public:
    /* 0x00 */ const char* mStageName;
    /* 0x04 */ const char* mStoryEventName;
    /* 0x08 */ bool _8;
};

class StorySequenceExecutorType {
public:
    class DemoSequenceInfo {
    public:
        /* 0x00 */ u16 _0;
        /* 0x02 */ u16 _2;
        /* 0x04 */ const char* _4;
    };
};

class StorySequenceExecutor : public NerveExecutor {
public:
    /// @brief Creates a new `StorySequenceExecutor`.
    StorySequenceExecutor();

    void update();
    bool isNeedMoviePlayerExecutingEventEnum() const;
    bool hasNextDemo() const;
    s32 getExecutingEventEnum() const;
    bool isEqualStageScenarioBefore(const char*, int) const;
    void moveGalaxy(GalaxyMoveArgument*, bool);
    void setNerveSceneStart();
    void forceStop();
    void exeIdle();
    void exeWaitToSceneStart();
    void exePlayDemoSequence();
    void exeWaitTimeKeepDemoEnd();
    void exeWaitSaveEnd();
    void exeStartSaveAfterSequence();
    void exeWaitSaveEndAfterSequence();
    void exeFadeinAfterSequence();
    void exeStaffRollSequence();
    void decideNextEventForClearGalaxy(GalaxyMoveArgument*);
    void decideNextEventForMoveGalaxy(GalaxyMoveArgument*);
    void decideNextEventForAfterLoading(GalaxyMoveArgument*) NO_INLINE;
    void decideNextStageForGalaxyOut(GalaxyMoveArgument*);
    void overwriteGalaxyNameAfterLoading(GalaxyMoveArgument*);
    void setNextStageToAstroGalaxyOrDome(GalaxyMoveArgument*);
    void setNextStageToAstroGalaxy(GalaxyMoveArgument*);
    void setNextStageToAstroDome(GalaxyMoveArgument*);
    const StorySequenceExecutorType::DemoSequenceInfo* isExecuteDemoLuigiMissing(const GalaxyMoveArgument*) const;
    void getOptionalDemoForClearGalaxy(MR::Vector< MR::FixedArray< const StorySequenceExecutorType::DemoSequenceInfo*, 8 > >*,
                                       const GalaxyMoveArgument*) const;
    static bool isEqualStageStopCometScheduler(const char*);
    void prepareDemoSequence(const StorySequenceExecutorType::DemoSequenceInfo*) NO_INLINE;
    void prepareDemoSequence(const MR::Vector< MR::FixedArray< const StorySequenceExecutorType::DemoSequenceInfo*, 8 > >&);
    void prepareDemoSequenceButlerFortressDiscover(const GalaxyMoveArgument*, const DemoFortressDiscoverCheckList&);
    bool tryStartDemo(const char*) NO_INLINE;
    bool tryStartSave();
    bool tryWaitSaveEnd();
    bool tryStartFadein();
    bool tryStartMovieAndWaitEnd(u32);
    const StorySequenceExecutorType::DemoSequenceInfo* getCurrentDemoInfo() const;
    bool tryNextDemoInfo();
    const StorySequenceExecutorType::DemoSequenceInfo* addDynamicDemoSequenceInfo(u16, u16, const char*);
    void setBeforeStageScenario(const GalaxyMoveArgument&, bool);
    s32 calcAproposScenarioNoOnAstroGalaxy() const;
    s32 calcAproposScenarioNoOnAstroDome() const;

    /* 0x08 */ const Nerve* mNextNerve;
    /* 0x0C */ NameObj* mDemoObj;
    /* 0x10 */ NameObj* mSaveObj;
    /* 0x14 */ char mStageName[48];
    /* 0x44 */ s32 mScenarioNo;
    /* 0x48 */ MR::Vector< MR::FixedArray< const StorySequenceExecutorType::DemoSequenceInfo*, 8 > > _48;
    /* 0x6C */ MR::Vector< MR::FixedArray< StorySequenceExecutorType::DemoSequenceInfo, 8 > > _6C;
    /* 0xB0 */ bool _B0;
};
