#pragma once

#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/Array.hpp"

class DemoFortressDiscoverCheckList;
class GalaxyMoveArgument;
class NameObj;

class StorySequenceExecutorType {
public:
    class DemoSequenceInfo {
    public:
        /* 0x00 */ u16 _0;
        /* 0x02 */ u16 _4;
        /* 0x04 */ const char* _8;
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
    void decideNextEventForAfterLoading(GalaxyMoveArgument*);
    void decideNextStageForGalaxyOut(GalaxyMoveArgument*);
    void overwriteGalaxyNameAfterLoading(GalaxyMoveArgument*);
    void setNextStageToAstroGalaxyOrDome(GalaxyMoveArgument*);
    void setNextStageToAstroDome(GalaxyMoveArgument*);
    bool isExecuteDemoLuigiMissing(const GalaxyMoveArgument*) const;
    void getOptionalDemoForClearGalaxy(MR::Vector<MR::FixedArray<const StorySequenceExecutorType::DemoSequenceInfo*, 8> >*, const GalaxyMoveArgument*) const;
    bool isEqualStageStopCometScheduler(const char*);
    void prepareDemoSequence(const StorySequenceExecutorType::DemoSequenceInfo*);
    void prepareDemoSequence(const MR::Vector<MR::FixedArray<const StorySequenceExecutorType::DemoSequenceInfo*, 8> >&);
    void prepareDemoSequenceButlerFortressDiscover(const GalaxyMoveArgument*, const DemoFortressDiscoverCheckList&);
    bool tryStartDemo(const char*);
    bool tryStartSave();
    bool tryWaitSaveEnd();
    bool tryStartFadein();
    bool tryStartMovieAndWaitEnd(u32);
    const StorySequenceExecutorType::DemoSequenceInfo* getCurrentDemoInfo() const;
    bool tryNextDemoInfo();
    const StorySequenceExecutorType::DemoSequenceInfo* addDynamicDemoSequenceInfo(u16, u16, const char*);
    void setBeforeStageScenario(const GalaxyMoveArgument&, bool);
    s32 calcAproposScenarioNoOnAstroGalaxy() const;

    /* 0x08 */ s32 _8;
    /* 0x0C */ NameObj* _C;
    /* 0x10 */ NameObj* _10;
    /* 0x14 */ char _14[48];
    /* 0x44 */ s32 _44;
    /* 0x48 */ MR::Vector<MR::FixedArray<const StorySequenceExecutorType::DemoSequenceInfo*, 8> > _48;
    /* 0x6C */ MR::Vector<MR::FixedArray<StorySequenceExecutorType::DemoSequenceInfo, 8> > _6C;
    /* 0xB0 */ bool _B0;
};
