#pragma once

class RushEndInfo;
class MarioActor;

class MarioAccess {
public:
    static void readyRemoteDemo();
    static void endRemoteDemo(const RushEndInfo*);
    static MarioActor* getPlayerActor();
};
