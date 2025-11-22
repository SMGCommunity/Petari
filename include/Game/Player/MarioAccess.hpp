#pragma once

class RushEndInfo;

class MarioAccess {
public:
    static void readyRemoteDemo();
    static void endRemoteDemo(const RushEndInfo*);
};
