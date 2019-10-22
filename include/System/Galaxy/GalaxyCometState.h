#ifndef GALAXYCOMETSTATE_H
#define GALAXYCOMETSTATE_H

#include "Actor/Nerve/NerveExecutor.h"

class GalaxyCometState : public NerveExecutor
{
public:
    GalaxyCometState();

    virtual ~GalaxyCometState();

    void update();
    bool isLand() const;
    bool isHide() const;
    void forceToNext();
    s32 getStateIndex() const;
    s32 getPastSecond() const;
    void setStateAndPastSecond(s32, u16);
};

#endif // GALAXYCOMETSTATE_H