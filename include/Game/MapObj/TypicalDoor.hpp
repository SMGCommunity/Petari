#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class CollisionParts;

class TypicalDoor : public MapObjActor {
public:
    TypicalDoor(const char*);

    /* 0x08 */ virtual ~TypicalDoor();
    /* 0x0C */ virtual void init(const JMapInfoIter&) override;
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) override;

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

    /* 0x0C */ virtual void init(const JMapInfoIter&) override;
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) override;
};

class DarknessRoomDoor : public TypicalDoor {
public:
    DarknessRoomDoor(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&) override;
    /* 0x48 */ virtual void control() override;
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&) override;

    void validateCollision();
    void invalidateCollision();

    /* 0xCC */ bool _CC;
};
