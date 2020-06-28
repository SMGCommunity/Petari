#include "Actor/State/ActorStateBase.h"

bool ActorStateBaseInterface::update()
{
    updateNerve();

    if (_8)
    {
        return 1;
    }
    
    control();
    return 0;
}