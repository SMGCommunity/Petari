#pragma once

#include "types.h"
#include "jmap/JMapInfoIter.h"

class NameObj
{
public:
    NameObj(const char *);

    virtual ~NameObj();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void calcViewAndEntry();

    void initWithoutIter();
    void setName(const char *);
    void executeMovement();
    void requestSuspend();
    void requestResume();
    void syncWithFlags();

    const char* mName; // _4
    u16 mFlags; // _8
    s16 _A;
};

class NameObjFunction
{
public:
    static void requestMovementOn(NameObj *);
    static void requestMovementOff(NameObj *);
};

class NameObjHolder
{
public:
    NameObjHolder();

    void add(NameObj *);
};

class NameObjRegister
{
public:
    NameObjRegister();

    void add(NameObj *);
    void setCurrentHolder(NameObjHolder *);

    NameObjHolder* mHolder;
};

class NameObjGroup : public NameObj
{
public:
    NameObjGroup(const char *, s32);

    virtual ~NameObjGroup();

    void registerObj(NameObj *);
    void pauseOffAll() const;
    void initObjArray(s32);

    s32 mNumObjs; // _C
    u32 _10;
    NameObj* mObjs; // _14
};