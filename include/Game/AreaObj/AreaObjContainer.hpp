#pragma once

#include "Game/NameObj/NameObj.hpp"

class AreaObj;
class AreaObjMgr;

typedef AreaObjMgr* (*ManagerCreatorFuncPtr)(s32, const char *);

struct Name2CreateManagerFunc {
    /* 0x0 */ const char* mName;
    /* 0x4 */ s32 _4;
    /* 0x8 */ ManagerCreatorFuncPtr mCreateFunc;
};

class AreaObjContainer : public NameObj {
public:
    /// @brief Creates a new `AreaObjContainer`.
    /// @param pName A pointer to the null-terminated name of the object.
    AreaObjContainer(const char* pName);

    /// @brief Destroys the `AreaObjContainer`.
    virtual ~AreaObjContainer();

    /// @brief Intializes the `AreaObjContainer` while being placed into a scene.
    /// @param rIter The reference to an iterator over a `JMapInfo`.
    virtual void init(const JMapInfoIter& rIter);

    AreaObjMgr* getManager(const char *) const;
    AreaObj* getAreaObj(const char *, const TVec3f &) const;

private:
    /* 0x00C */ AreaObjMgr* mManagerArray[0x50];
    /* 0x14C */ u32 mNumManagers;
};

namespace MR {
    AreaObjContainer* getAreaObjContainer();
};
