#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Screen/LayoutActor.hpp"

class PlayerRacer;
class RaceManager;

class RaceManagerLayout : public LayoutActor {
public:
    /// @brief Creates a new `RaceManagerLayout`.
    /// @param pName A pointer to the null-terminated name of the object.
    RaceManagerLayout(const char* pName);

    /// @brief Intializes the `RaceManagerLayout` while being placed into a scene.
    /// @param rIter The reference to an iterator over a `JMapInfo`.
    virtual void init(const JMapInfoIter& rIter);

    void playCountAndGo();
    void playGo();
    void playGoal();
    void playLose();
    void playTimeUp();
    void playRank(u32);
    bool isAllAnimStopped();
    void hideRecordPane();
    void hideBestRecordPane();
    void hideAllPane();
    bool isPlayCountAnim() const;
    bool isPlayCountAndGoAnim() const;
    void setTime(u32);
    void setBestTime(u32);
};

class AbstractRacer {
public:
    /// @brief Creates a new `AbstractRacer`.
    AbstractRacer();

    virtual void initRacer();
    virtual void prepRacer(const RaceManager* pRaceManager) = 0;
    virtual void startRacer() = 0;
    virtual bool updateRacer(const RaceManager* pRaceManager) = 0;
    virtual bool goalRacer() = 0;
    virtual void loseRacer() = 0;
    virtual void resetRacer(const RaceManager* pRaceManager) = 0;
    virtual void exitRacer() = 0;

    /* 0x04 */ f32 _4;
    /* 0x08 */ f32 _8;
    /* 0x0C */ TVec3f mCurrPosition;
    /* 0x18 */ TVec3f mPrevPosition;
};

class AbstractAudience {
public:
    /// @brief Creates a new `AbstractAudience`.
    AbstractAudience();

    virtual void prepAudience() = 0;
    virtual void resetAudience() = 0;

    /* 0x04 */ u8 _4;
    /* 0x08 */ s32 _8;
    /* 0x0C */ s32 _C;
    /* 0x10 */ s32 _10;
};

class RaceManager : public LiveActor {
public:
    /// @brief Creates a new `RaceManager`.
    RaceManager();

    /// @brief Intializes the `RaceManager` while being placed into a scene.
    /// @param rIter The reference to an iterator over a `JMapInfo`.
    virtual void init(const JMapInfoIter& rIter);

    virtual void movement();

    void exeWait();
    void exePrep();
    void exeWipeOut();
    void exeWipeWait();
    void exeWipeIn();
    void exeIntro();
    void exeCount();
    void exeRace();
    void exeDemo();
    void exeGoal();
    void exeRank();
    bool startWithWipe();
    bool startImmediately();
    void prepRace();
    void startRace();
    void goalRace();
    void resetRace();
    void prepBind();
    void entry(AbstractRacer*);
    void entry(PlayerRacer*);
    void entry(AbstractAudience*);
    bool isGoal(const AbstractRacer*) const;
    u32 getRank(const AbstractRacer*) const;
    void renewTime();

    /* 0x8C */ s32 mCurrentRace;
    /* 0x90 */ RaceManagerLayout* mLayout;
    /* 0x94 */ AbstractRacer* mRacer[10];
    /* 0xBC */ AbstractAudience* mAudience[10];
    /* 0xE4 */ u32 mRacerNum;
    /* 0xE8 */ u32 mAudienceNum;
    /* 0xEC */ u32 mRank;
    /* 0xF0 */ u32 mBestTime;
    /* 0xF4 */ u32 mTime;
    /* 0xF8 */ PlayerRacer* mPlayerRacer;
    /* 0xFC */ TPos3f _FC;
};

namespace RaceManagerFunction {
    void entryRacerOthers(AbstractRacer*);
    void entryAudience(AbstractAudience*);
    void entryRacerPlayer(PlayerRacer*);
    void startRaceWithWipe();
    void startRaceImmediately();
    u32 getRaceRank();
    u32 getRaceTime();
    const char* getRaceName(int);
    const char* getRaceMessageId(int);
    s32 getRaceId(const char*, s32);
    bool hasPowerStarRaceScenario(int);
};  // namespace RaceManagerFunction
