#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class JMapIdInfo;

class RestartCube : public AreaObj {
public:
    RestartCube(int, const char*);

    /* 0x08 */ virtual ~RestartCube();
    /* 0x0C */ virtual void init(const JMapInfoIter&);

    void updatePlayerRestartIdInfo();
    void changeBgm();

    /* 0x3C */ JMapIdInfo* mIdInfo;
    /* 0x40 */ s32 _40;
    /* 0x44 */ s32 _44;
    /* 0x48 */ bool _48;
};
