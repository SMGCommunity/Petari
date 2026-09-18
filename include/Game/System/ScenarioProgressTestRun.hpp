#pragma once

class GameDataHolder;

class ScenarioProgressTestRun {
public:
    /// @brief Creates a new `ScenarioProgressTestRun`.
    /// @param pHost The pointer to the owning container for persistent gameplay data.
    ScenarioProgressTestRun(GameDataHolder* pHost);

public:
    /// @brief The pointer to the owning container for persistent gameplay data.
    /* 0x00 */ GameDataHolder* mHost;
};
