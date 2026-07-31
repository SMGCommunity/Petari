#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class CollisionParts;

class TypicalDoor : public MapObjActor {
public:
    TypicalDoor(const char*);

    virtual ~TypicalDoor();
    virtual void init(const JMapInfoIter&) override;
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) override;

    void open();
    void close();
    void exeClose();
    void exeOpen();

    /* 0xC4 */ CollisionParts* mCloseCollision;
    /* 0xC8 */ CollisionParts* mOpenCollision;
};

class TypicalDoorOpen : public TypicalDoor {
public:
    TypicalDoorOpen(const char* pName) : TypicalDoor(pName) {
    }

    virtual void init(const JMapInfoIter&) override;
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) override;
};

class DarknessRoomDoor : public TypicalDoor {
public:
    DarknessRoomDoor(const char*);

    virtual void init(const JMapInfoIter&) override;
    virtual void control() override;
    virtual void connectToScene(const MapObjActorInitInfo&) override;

    void validateCollision();
    void invalidateCollision();

    /* 0xCC */ bool _CC;
};
