#pragma once

#include "Game/NameObj/NameObj.h"

class NameObjExecuteInfo {
public:
    NameObjExecuteInfo();

    void requestMovementOn(int);
    void requestMovementOff(int);

    u32 _0;
    u8 _4;
    u8 _5;
    s8 _6;
    s8 _7;
    s8 _8;
    s8 _9;
    s8 _A;
};

class NameObjExecuteHolder : public NameObj { 
public:
    NameObjExecuteHolder(int);

    virtual ~NameObjExecuteHolder();

    void registerActor(NameObj *, int, int, int, int);
    void initConnectting();

    void requestMovementOn(int);
    void requestMovementOff(int);
    NameObjExecuteInfo* getConnectToSceneInfo(const NameObj *) const;

    NameObjExecuteInfo* mExecuteArray;  // _C
    u32 mExecuteCount;                  // _10
    u32 _14;
    u8 _18;
    u8 _19;
    u8 _1A;
    u8 _1B;
    u8 _1C;
};