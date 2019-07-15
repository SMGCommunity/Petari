#include "Actor/LiveActor/AllLiveActorGroup.h"

#include "Actor/Clipping/ClippingDirector.h"

AllLiveActorGroup::AllLiveActorGroup() : LiveActorGroup("AllLiveActorGroup", 0xA00)
{
    this->_18 = 0;
}

void AllLiveActorGroup::initActorSystemInfo(const JMapInfoIter &iter)
{
    if (this->mNumObjs <= 0)
    {
        return;
    }

    u32 r31 = this->_18;

    if (r31 == this->mNumObjs)
    {
        return;
    }
    else
    {
        while (r31 < this->mNumObjs)
        {
            LiveActor* actor = this->getActor(this->_18);
            ClippingDirector* director = MR::getClippingDirector();
            director->initActorSystemInfo(actor, iter);
            r31++;
            this->_18++;
        }
    }
    
}