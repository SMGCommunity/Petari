#pragma once

/// @brief The evaluator for a change in Boolean value.
class TriggerChecker {
public:
    /// @brief Creates a new `TriggerChecker`.
    TriggerChecker();

    /// @brief Updates the state of the trigger.
    /// @param input The new state of the trigger.
    void update(bool input);

    /// @brief Initializes the state of the trigger.
    /// @param input The new state of the trigger.
    void setInput(bool input);

    /// @brief Returns the current state of the trigger.
    /// @return The current state of the trigger.
    bool getLevel() const;

    /// @brief Determines if the trigger was just turned on.
    /// @return `true` if the trigger was just turned on, `false` otherwise.
    bool getOnTrigger() const;

    /// @brief Determines if the trigger was just turned off.
    /// @return `true` if the trigger was just turned off, `false` otherwise.
    bool getOffTrigger() const;

public:
    /// @brief The previous state of the trigger.
    /* 0x00 */ bool mPrevLevel;

    /// @brief The current state of the trigger.
    /* 0x01 */ bool mCurrLevel;
};
