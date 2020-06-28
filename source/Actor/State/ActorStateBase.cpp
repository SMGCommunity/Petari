#include "Actor/State/ActorStateBase.h"

void ActorStateBaseInterface::control()
{
    
}

void ActorStateBaseInterface::kill()
{
    mIsDead = 1;
}

void ActorStateBaseInterface::appear()
{
    mIsDead = 0;
}

void ActorStateBaseInterface::init()
{

}

bool ActorStateBaseInterface::update()
{
    updateNerve();

    if (mIsDead)
    {
        return 1;
    }
    
    control();
    return 0;
}