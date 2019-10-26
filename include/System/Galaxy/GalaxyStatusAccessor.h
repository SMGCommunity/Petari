#ifndef GALAXYSTATUSACCESSOR_H
#define GALAXYSTATUSACCESSOR_H

#include "types.h"

class GalaxyStatusAccessor;

namespace MR
{
    static GalaxyStatusAccessor* makeGalaxyStatusAcessor(const char *);
};

class GalaxyStatusAccessor
{
public:
    const char* getCometName(s32) const;
};

#endif // GALAXYSTATUSACCESSOR_H