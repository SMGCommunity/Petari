#pragma once

#include "Game/Util/JMapInfo.hpp"
#include <revolution/types.h>

/// @brief The most basic form of an object.
class NameObj {
public:
    /// @brief Creates a new `NameObj`.
    /// @param pName A pointer to the null-terminated name of the object.
    NameObj(const char* pName);

    /// @brief Destroys the `NameObj`.
    virtual ~NameObj();

    /// @brief Intializes the `NameObj` while being placed into a scene.
    /// @param rIter A reference to an iterator over a `JMapInfo`.
    virtual void init(const JMapInfoIter& rIter);

    /// @brief Intializes the `NameObj` after being placed into a scene.
    virtual void initAfterPlacement();

    virtual void movement();

    /// @brief Draws the `NameObj` to the screen.
    virtual void draw() const;

    virtual void calcAnim();
    virtual void calcViewAndEntry();

    /// @brief Initializes the `NameObj` without an iterator over a `JMapInfo`.
    void initWithoutIter();

    /// @brief Updates the name of the `NameObj`.
    /// @param pName A pointer to the new null-terminated name of the object.
    void setName(const char* pName);

    /// @brief Returns the name of the `NameObj`.
    /// @return A pointer to the null-terminated name of the object.
    const char* getName() const {
        return mName;
    }

    u16 getFlag() const {
        return mFlag;
    }

    void executeMovement();
    void requestSuspend();
    void requestResume();
    void syncWithFlags();

    /* 0x04 */ const char* mName;  ///< A string to identify the NameObj.
    /* 0x08 */ u16 mFlag;          ///< Flags in relation to movement.
    /* 0x0A */ s16 mExecutorIdx;   ///< The index into the NameObjExecuteInfo array.
};

/// @brief Contains static functions to begin and end movement in a NameObj.
class NameObjFunction {
public:
    static void requestMovementOn(NameObj*);
    static void requestMovementOff(NameObj*);
};
