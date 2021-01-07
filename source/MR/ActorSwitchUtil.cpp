#include "MR/ActorSwitchUtil.h"
#include "MR/JMapUtil.h"

namespace MR
{
    bool useStageSwitchReadA(LiveActor *pActor, const JMapInfoIter &iter)
    {
        bool ret;

        if (iter.isValid() == 0)
        {
            ret = 0;
        }
        else
        {
            if (MR::isExistStageSwitchA(iter) == 0)
            {
                ret = 0;
            }
            else
            {
                if (pActor->mStageSwitchCtrl == 0)
                {
                    pActor->initStageSwitch(iter);
                }

                ret = 1;
            }
        }

        return ret;
    }

    bool useStageSwitchReadB(LiveActor *pActor, const JMapInfoIter &iter)
    {
        bool ret;

        if (iter.isValid() == 0)
        {
            ret = 0;
        }
        else
        {
            if (MR::isExistStageSwitchB(iter) == 0)
            {
                ret = 0;
            }
            else
            {
                if (pActor->mStageSwitchCtrl == 0)
                {
                    pActor->initStageSwitch(iter);
                }

                ret = 1;
            }
        }

        return ret;
    }

    bool useStageSwitchReadAppear(LiveActor *pActor, const JMapInfoIter &iter)
    {
        bool ret;

        if (iter.isValid() == 0)
        {
            ret = 0;
        }
        else
        {
            if (MR::isExistStageSwitchAppear(iter) == 0)
            {
                ret = 0;
            }
            else
            {
                if (pActor->mStageSwitchCtrl == 0)
                {
                    pActor->initStageSwitch(iter);
                }

                ret = 1;
            }
        }

        return ret;
    }
};