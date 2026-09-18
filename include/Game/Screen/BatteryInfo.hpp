#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/System/NerveExecutor.hpp"

class BatteryInfo : public NerveExecutor {
public:
    /// @brief Creates a new `BatteryInfo`.
    /// @param pHost The pointer to the owning actor instance.
    /// @param pPaneName The pointer to the null-terminated name of the pane.
    /// @param channel The handle to the Wii Remote.
    BatteryInfo(LayoutActor* pHost, const char* pPaneName, int channel);

    /// @brief Updates the object when the popup is hidden for a sufficient battery level.
    void exeEnought();

    /// @brief Updates the object when the popup for a low battery level is transitioning on screen.
    void exeLowAppear();

    /// @brief Updates the object when the popup for a low battery level is idle.
    void exeLow();

    /// @brief Updates the object when the popup for a low battery level is transitioning off screen.
    void exeLowDisappear();

    /// @brief Updates the object when the popup for a low battery is hidden.
    void exeLowHide();

    void exeDisappear();

    /// @brief Updates the object when the popup for a critical battery level is transitioning on screen.
    void exeCriticalAppear();

    /// @brief Updates the object when the popup for a critical battery level is idle.
    void exeCritical();

    /// @brief Updates the object when the popup for a depleted battery level is transitioning on screen.
    void exeNoneAppear();

    /// @brief Updates the object when the popup for a depleted battery level is idle.
    void exeNone();

    /// @brief Updates the object when the popup for a depleted battery level is transitioning off screen.
    void exeNoneDisappear();

    /// @brief Updates the object when the popup for a depleted battery level is hidden.
    void exeNoneHide();

    /// @brief Plays the given animation and transitions from the working nerve action into the given nerve action once the animation has stopped.
    /// @param pAnimName The pointer to the null-terminated name of the animation.
    /// @param pNerve The pointer to the new nerve action.
    /// @return `true` if the new nerve action was set, `false` otherwise.
    bool exeStartAnimAndSetNerveIfStopped(const char* pAnimName, const Nerve* pNerve);

    /// @brief Transitions from the working nerve action into a suitable nerve action, if the battery level changes while the popup is idle.
    /// @return `true` if a new nerve action was set, `false` otherwise.
    bool tryChangeNerveWithBatteryLeftAlreadyAppear();

public:
    /// @brief The pointer to the owning actor instance.
    /* 0x08 */ LayoutActor* mHost;

    /// @brief The pointer to the null-terminated name of the pane.
    /* 0x0C */ const char* mPaneName;

    /// @brief The handle to the Wii Remote.
    /* 0x10 */ int mChannel;
};

/// @brief The actor for the popup warning the player about their Wii Remote's battery level.
class BatteryLayout : public LayoutActor {
public:
    /// @brief Creates a new `BatteryLayout`.
    BatteryLayout();

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

public:
    /* 0x20 */ BatteryInfo* mInfo1P;
    /* 0x24 */ BatteryInfo* mInfo2P;
};
