#include "MR/actor/ActorSwitchUtil.h"
#include "MR/JMap/JMapUtil.h"

namespace MR
{
    bool useStageSwitchReadA(LiveActor *actor, const JMapInfoIter &iter)
    {
        bool ret;

        if (iter.isValid() == 0)
            ret = 0;
        else
        {
            if (MR::isExistStageSwitchA(iter) == 0)
                ret = 0;
            else
            {
                if (actor->mStageSwitchCtrl == 0)
                    actor->initStageSwitch(iter);

                ret = 1;
            }
        }

        return ret;
    }

    bool useStageSwitchReadB(LiveActor *actor, const JMapInfoIter &iter)
    {
        bool ret;

        if (iter.isValid() == 0)
            ret = 0;
        else
        {
            if (MR::isExistStageSwitchB(iter) == 0)
                ret = 0;
            else
            {
                if (actor->mStageSwitchCtrl == 0)
                    actor->initStageSwitch(iter);

                ret = 1;
            }
        }

        return ret;
    }

    bool useStageSwitchReadAppear(LiveActor *actor, const JMapInfoIter &iter)
    {
        bool ret;

        if (iter.isValid() == 0)
            ret = 0;
        else
        {
            if (MR::isExistStageSwitchAppear(iter) == 0)
                ret = 0;
            else
            {
                if (actor->mStageSwitchCtrl == 0)
                    actor->initStageSwitch(iter);

                ret = 1;
            }
        }

        return ret;
    }
};