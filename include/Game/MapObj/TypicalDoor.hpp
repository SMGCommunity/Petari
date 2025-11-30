#pragma once

#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"

class TypicalDoor : public MapObjActor {
public:
    TypicalDoor(const char*);

    virtual ~TypicalDoor();
    virtual void init(const JMapInfoIter&) override;
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) override;

    void exeClose();
    void exeOpen();

    void open();
    void close();
    inline void listenForClose();

    CollisionParts* mCloseCollision;  // 0xC4
    CollisionParts* mOpenCollision;   // 0xC8
};

class TypicalDoorOpen : public TypicalDoor {
public:
    inline TypicalDoorOpen(const char* pName) : TypicalDoor(pName) {}

    virtual ~TypicalDoorOpen();
    virtual void init(const JMapInfoIter&) override;
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) override;
};

class DarknessRoomDoor : public TypicalDoor {
public:
    DarknessRoomDoor(const char*);

    void validateCollision();
    void invalidateCollision();

    virtual ~DarknessRoomDoor();
    virtual void init(const JMapInfoIter&) override;
    virtual void control() override;
    virtual void connectToScene(const MapObjActorInitInfo&) override;

    bool _CC;
};
