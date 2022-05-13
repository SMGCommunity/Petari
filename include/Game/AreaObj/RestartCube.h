#pragma once

#include "Game/AreaObj/AreaObj.h"

class RestartCube : public AreaObj {
public:
    RestartCube(int, const char *);
    virtual ~RestartCube();

    virtual void init(const JMapInfoIter &);

    void updatePlayerRestartIdInfo();
    void changeBgm();

    JMapIdInfo *mIdInfo;    // _3C
    s32 _40;
    s32 _44;
    bool _48;
    u8 _49[3];
};
