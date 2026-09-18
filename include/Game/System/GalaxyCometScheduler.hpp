#pragma once

#include "Game/Util/Array.hpp"

class GalaxyCometState;

struct GalaxyCometSerializeInfo {
    /* 0x0 */ u16 mStateIndexPacked;
    /* 0x2 */ u16 mPastSecond;
};

struct GalaxyCometTimePaper {
    /* 0x0 */ const char* mGalaxyName;
    /* 0x4 */ s32 mCategory;
};

class GalaxyCometTimeTable {
public:
    /// @brief Initializes the table with the supplied time paper sequence.
    /// @param pTimePaper Pointer to the time paper entries.
    GalaxyCometTimeTable(const GalaxyCometTimePaper*);

    /// @brief Ticks the comet state and updates the cursor when ready.
    void update();

    /// @brief Refreshes readiness by scanning for the next eligible entry.
    void checkIsReady();

    /// @brief Packs the current cursor and timing state into the provided game data struct.
    /// @param pInfo Output structure that receives the serialized state.
    void serializeStateToGameData(GalaxyCometSerializeInfo*);

    /// @brief Restores the cursor and timing state from serialized game data.
    /// @param pInfo Input structure that supplies the serialized state.
    void deserializeStateFromGameData(const GalaxyCometSerializeInfo*);

    /// @brief Reports whether the current comet entry is active in orbit.
    /// @return `true` when the current entry is landed, otherwise `false`.
    bool isLand() const;

    /// @brief Checks if the given galaxy exists anywhere in this time paper.
    /// @param galaxyName Galaxy name to test.
    /// @return `true` if the galaxy is present, `false` otherwise.
    bool isIncluded(const char*) const;

    /// @brief Returns the category for the current time paper entry.
    /// @return Active entry category identifier.
    s32 getCometCategory() const;

    /// @brief Adjusts the paper cursor after state changes, honoring hide status.
    void updateTimePaperPos();

    /// @brief Searches forward for the next entry whose comet can appear.
    /// @param pResultPos Output pointer that receives the viable position.
    /// @param startPos Cursor index to begin searching from.
    /// @return `true` if a suitable position was found, `false` otherwise.
    bool findSuitableTimePaperPos(s32*, s32) const;

    /// @brief Counts the populated entries within the time paper.
    /// @return Number of entries (excluding null terminator).
    s32 calcTimePaperLength() const;

    /// @brief Forces the current state forward and reapplies cursor adjustment.
    void advance();

    /* 0x0 */ GalaxyCometState* mState;
    /* 0x4 */ const GalaxyCometTimePaper* mTimePaper;
    /* 0x8 */ s32 mTimePaperPos;
    /* 0xC */ bool mIsReady;
    /* 0xD */ bool mIsHide;
};

class GalaxyCometScheduler {
public:
    /// @brief Constructs the scheduler with the predefined dome time tables.
    GalaxyCometScheduler();

    /// @brief Ticks every time table while scheduling is enabled.
    void update();

    /// @brief Enables periodic updates of all time tables.
    void activate();

    /// @brief Disables periodic updates of all time tables.
    void deactivate();

    /// @brief Advances every time table and ensures comet progression constraints.
    void forceToNextState();

    /// @brief Refreshes readiness and reacts to comet-related game flags.
    void syncWithFlags();

    /// @brief Serializes every time table into persistent game data.
    void updateStateToGameData();

    /// @brief Restores every time table from persistent game data.
    /// @note Overrides the Terrace Dome comet state to force Egg Star Galaxy's 100-coin comet if unlocked.
    void restoreStateFromGameData();

    /// @brief Loads the predefined first-contact schedule into each time table.
    void setScheduleFirstContact();

    /// @brief Promotes the first ready table to its next state when all comets are hidden.
    void setScheduleMostForwardCometAppear();

    /// @brief Checks if the named galaxy currently hosts an active comet.
    /// @param pGalaxyName Galaxy name to test.
    /// @return `true` if the comet is active in orbit in that galaxy, otherwise `false`.
    bool isCometLand(const char*) const;

    /// @brief Resolves the power star ID tied to the active comet category.
    /// @param pGalaxyName Galaxy name used to locate the relevant table.
    /// @return Power star ID for the active comet, or `0` if unavailable.
    s32 getEncounterCometPowerStarId(const char*) const;

    /// @brief Returns the localized name for the active comet in the given galaxy.
    /// @param pGalaxyName Galaxy name that hosts the comet.
    /// @return Localized comet name string.
    const char* getEncounterCometName(const char*) const;

    /// @brief Finds the time table whose current entry targets the named galaxy.
    /// @param pGalaxyName Galaxy name to locate among current entries.
    /// @return Matching time table pointer, or `nullptr` when not found.
    GalaxyCometTimeTable* findFromGalaxy(const char*) const;

    /// @brief Tests whether no time table reports an active comet.
    /// @return `true` if every table is currently hidden, otherwise `false`.
    bool isHideAll() const;

    /// @brief Returns the packed state value for the named galaxy when present.
    /// @param pGalaxyName Galaxy name to query within each time table.
    /// @return Packed state value, or `-1` when the galaxy is absent.
    s32 getStateValueIncluded(const char*);

public:
    /* 0x0 */ MR::AssignableArray< GalaxyCometTimeTable* > mTimeTables;
    /* 0x8 */ bool mEnabled;
};
