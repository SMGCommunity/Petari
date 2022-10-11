#pragma once

#include "Game/NameObj/NameObj.h"

/// @brief Class that can contain multiple NameObj instances stored in a group.
class NameObjGroup : public NameObj {
public:
    NameObjGroup(const char *, int);

    virtual ~NameObjGroup();

    void registerObj(NameObj *);
    void pauseOffAll() const;
    void initObjArray(int);

    s32 _C;
    s32 mObjectCount;   // _10
    NameObj** mObjects; // _14
};