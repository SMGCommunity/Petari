#pragma once

#include "Game/NameObj/NameObj.hpp"

/// @brief Class that can contain multiple NameObj instances stored in a group.
class NameObjGroup : public NameObj {
public:
    /// @brief Creates a new `NameObjGroup`.
    /// @param pName A pointer to the null-terminated name of the object.
    /// @param numMax The maximum allowed number of `NameObj` instances in the group.
    NameObjGroup(const char* pName, int numMax);

    /// @brief Destroys the `NameObjGroup`.
    virtual ~NameObjGroup();

    NameObj* getObj(int index) const {
        return mObjArray[index];
    }

    s32 getObjNum() const {
        return mObjNum;
    }

    void registerObj(NameObj*);
    void pauseOffAll() const;
    void initObjArray(int);

private:
    /* 0x0C */ s32 mObjNumMax;
    /* 0x10 */ s32 mObjNum;
    /* 0x14 */ NameObj** mObjArray;
};
