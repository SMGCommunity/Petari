#pragma  once

#include "Game/LiveActor/LiveActor.hpp"

class NPCSupportRail : LiveActor {
public: 
    NPCSupportRail(const char *);

    virtual ~NPCSupportRail();
    virtual void init(const JMapInfoIter &);
};
