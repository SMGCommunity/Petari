#include "Util/ActorSwitchUtil.h"
#include "Util/JMapUtil.h"

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

    bool useStageSwitchWriteA(LiveActor *pActor, const JMapInfoIter &iter)
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

    bool useStageSwitchWriteB(LiveActor *pActor, const JMapInfoIter &iter)
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

    bool useStageSwitchWriteDead(LiveActor *pActor, const JMapInfoIter &iter)
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

    bool needStageSwitchReadA(LiveActor *pActor, const JMapInfoIter &iter)
    {
        useStageSwitchReadA(pActor, iter);
    } 

    bool needStageSwitchReadB(LiveActor *pActor, const JMapInfoIter &iter)
    {
        useStageSwitchReadB(pActor, iter);
    } 

    bool needStageSwitchReadAppear(LiveActor *pActor, const JMapInfoIter &iter)
    {
        useStageSwitchReadAppear(pActor, iter);
    } 

    bool needStageSwitchWriteA(LiveActor *pActor, const JMapInfoIter &iter)
    {
        useStageSwitchWriteA(pActor, iter);
    } 

    bool needStageSwitchWriteB(LiveActor *pActor, const JMapInfoIter &iter)
    {
        useStageSwitchWriteA(pActor, iter);
    } 

    bool needStageSwitchWriteDead(LiveActor *pActor, const JMapInfoIter &iter)
    {
        useStageSwitchWriteDead(pActor, iter);
    } 

    bool isValidSwitchA(const LiveActor *pActor)
    {
        bool ret = false;

        if (pActor->mStageSwitchCtrl && pActor->mStageSwitchCtrl->isValidSwitchA())
        {
            ret = true;
        }

        return ret;
    }

    bool isValidSwitchB(const LiveActor *pActor)
    {
        bool ret = false;

        if (pActor->mStageSwitchCtrl && pActor->mStageSwitchCtrl->isValidSwitchB())
        {
            ret = true;
        }

        return ret;
    }

    bool isValidSwitchAppear(const LiveActor *pActor)
    {
        bool ret = false;

        if (pActor->mStageSwitchCtrl && pActor->mStageSwitchCtrl->isValidSwitchAppear())
        {
            ret = true;
        }

        return ret;
    }

    bool isValidSwitchDead(const LiveActor *pActor)
    {
        bool ret = false;

        if (pActor->mStageSwitchCtrl && pActor->mStageSwitchCtrl->isValidSwitchDead())
        {
            ret = true;
        }

        return ret;
    }

    bool isOnSwitchA(const LiveActor *pActor)
    {
        pActor->mStageSwitchCtrl->isOnSwitchA();
    }

    bool isOnSwitchB(const LiveActor *pActor)
    {
        pActor->mStageSwitchCtrl->isOnSwitchA();
    }

    bool isOnSwitchAppear(const LiveActor *pActor)
    {
        pActor->mStageSwitchCtrl->isOnSwitchA();
    }

    void onSwitchA(LiveActor *pActor)
    {
        pActor->mStageSwitchCtrl->onSwitchA();
    }

    void onSwitchB(LiveActor *pActor)
    {
        pActor->mStageSwitchCtrl->onSwitchB();
    }

    void onSwitchDead(LiveActor *pActor)
    {
        pActor->mStageSwitchCtrl->onSwitchDead();
    }

    void offSwitchA(LiveActor *pActor)
    {
        pActor->mStageSwitchCtrl->offSwitchA();
    }

    void offSwitchB(LiveActor *pActor)
    {
        pActor->mStageSwitchCtrl->offSwitchB();
    }

    void offSwitchDead(LiveActor *pActor)
    {
        pActor->mStageSwitchCtrl->offSwitchDead();
    }
};