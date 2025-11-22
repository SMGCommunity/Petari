#pragma once

#include "Game/NameObj/NameObj.hpp"

/// @brief Class that can contain multiple NameObj instances stored in a group.
class NameObjGroup : public NameObj {
public:
    NameObjGroup(const char*, int);

    virtual ~NameObjGroup();

    void registerObj(NameObj*);
    void pauseOffAll() const;
    void initObjArray(int);

    s32 getObjectCount() const { return mObjectCount; }

    s32 _C;
    s32 mObjectCount;    // 0x10
    NameObj** mObjects;  // 0x14
};