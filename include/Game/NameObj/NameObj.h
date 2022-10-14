#pragma once

#include "Game/Util.h"

#include <revolution.h>

namespace MR {
    void notifyRequestNameObjMovementOnOff();
}

/// @brief The most basic form of an object.
class NameObj {
public:
    /// @brief Constructs a new NameObj instance.
    /// @param pName The new name assigned to the NameObj.
    NameObj(const char *pName);

    virtual ~NameObj();
    /// @brief Intializes the NameObj and can set various settings and construct necessary classes.
    /// @param rIter A reference to the JMapInfoIter that supplies BCSV information.
    virtual void init(const JMapInfoIter &rIter);
    virtual void initAfterPlacement();
    virtual void movement();
    /// @brief Draws the object. Does nothing until overridden.
    virtual void draw() const;
    virtual void calcAnim();
    virtual void calcViewAndEntry();

    /// @brief Initializes a NameObj without a JMapInfoIter instance.
    void initWithoutIter();
    /// @brief Sets the NameObj's mName.
    /// @param pName The new name assigned to the NameObj.
    void setName(const char *pName);
    void executeMovement();
    void requestSuspend();
    void requestResume();
    void syncWithFlags();

    /* 0x4 */   const char* mName;      ///< A string to identify the NameObj.
    /* 0x8 */   volatile u16 mFlags;    ///< Flags in relation to movement.
    /* 0xA */   s16 mExecutorIdx;       ///< The index into the NameObjExecuteInfo array.
};

/// @brief Contains static functions to begin and end movement in a NameObj.
class NameObjFunction {
public:
    static void requestMovementOn(NameObj *);
    static void requestMovementOff(NameObj *);
};