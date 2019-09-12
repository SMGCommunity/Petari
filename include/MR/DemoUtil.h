#ifndef DEMOUTIL_H
#define DEMOUTIL_H

#include "JMap/JMapInfoIter.h"

class LiveActor;
class Nerve;

namespace MR
{
    bool tryRegisterDemoCast(LiveActor *, const JMapInfoIter &);

    void registerDemoActionNerve(const LiveActor *, const Nerve *, const char *);
};

#endif // DEMOUTIL_H