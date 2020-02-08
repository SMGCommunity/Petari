#ifndef STAGESWITCHCTRL_H
#define STAGESWITCHCTRL_H

#include <revolution.h>
#include "Actor/NameObj/NameObj.h"
#include "JMap/JMapInfoIter.h"

class StageSwitchCtrl
{
public:
    StageSwitchCtrl(const JMapInfoIter &);

    u8 _0[0x10];
};

namespace MR
{
    StageSwitchCtrl* createStageSwitchCtrl(NameObj *, const JMapInfoIter &);
};

#endif // STAGESWITCHCTRL_H