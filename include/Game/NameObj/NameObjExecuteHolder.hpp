#pragma once

#include "Game/NameObj/NameObj.hpp"

class LiveActor;

class NameObjExecuteInfo {
public:
    NameObjExecuteInfo();

    void setConnectInfo(NameObj *, int, int, int, int);
    void initConnectting();
    void requestConnect(u8 *);
    void requestDisconnect(u8 *, bool);
    void executeRequirementConnectMovement();
    void executeRequirementDisconnectMovement();
    void executeRequirementConnectDraw();
    void executeRequirementDisconnectDraw();
    void executeRequirementDisconnectDrawDelay();
    void requestMovementOn(int);
    void requestMovementOff(int);
    void findLightInfo() const;
    void connectToScene();
    void disconnectToScene();
    void connectToDraw();
    void disconnectToDraw();

    NameObj* mExecutedObject;   // 0x0
    s8 _4;
    s8 _5;
    s8 _6;
    s8 _7;
    s8 _8;
    s8 _9;
    s16 _A;
};

class NameObjExecuteHolder : public NameObj { 
public:
    NameObjExecuteHolder(int);

    virtual ~NameObjExecuteHolder();

    void registerActor(NameObj *, int, int, int, int);
    void initConnectting();
    void connectToScene(NameObj *);
    void connectToDraw(NameObj *);
    void disconnectToScene(NameObj *);
    void disconnectToDraw(NameObj *);
    bool isConnectToDraw(const NameObj *) const;
    void executeRequirementConnectMovement();
    void executeRequirementDisconnectMovement();
    void executeRequirementConnectDraw();
    void executeRequirementDisconnectDraw();
    void executeRequirementDisconnectDrawDelay();
    void requestMovementOn(int);
    void requestMovementOff(int);
    NameObjExecuteInfo* getConnectToSceneInfo(const NameObj *) const;

    NameObjExecuteInfo* mExecuteArray;  // 0xC
    s32 mExecuteCount;                  // 0x10
    u32 _14;
    u8 _18;
    u8 _19;
    u8 _1A;
    u8 _1B;
    u8 _1C;
};

namespace MR {
    void registerNameObjToExecuteHolder(NameObj *, int, int, int, int);
    void initConnectting();
    void connectToSceneTemporarily(NameObj *);
    void disconnectToSceneTemporarily(NameObj *);
    void connectToDrawTemporarily(NameObj *);
    void disconnectToDrawTemporarily(NameObj *);
    bool isConnectToDrawTemporarily(const NameObj *);
    void executeRequirementConnectMovement();
    void executeRequirementDisconnectMovement();
    void executeRequirementConnectDraw();
    void executeRequirementDisconnectDraw();
    void executeRequirementDisconnectDrawDelay();
    void requestMovementOnWithCategory(int);
    void requestMovementOffWithCategory(int);
    void findActorLightInfo(const LiveActor *);
};
