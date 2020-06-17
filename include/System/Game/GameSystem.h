#ifndef GAMESYSTEM_H
#define GAMESYSTEM_H

#include "Actor/Nerve/NerveExecutor.h"

#include <nw4r/lyt.h>
#include <revolution.h>
#include <__ppc_eabi_init.h>

#ifdef __cplusplus
extern "C" {
#endif

void main();

#ifdef __cplusplus
}
#endif

class GameSystem : public NerveExecutor
{
public:
    GameSystem();
};

#endif // GAMESYSTEM_H