#include "LiveActor/FlashingCtrl/FlashingCtrl.h"
#include "Util/LiveActorUtil.h"
#include "Util/ObjUtil.h"
#include "Util/DemoUtil.h"

FlashingCtrl::FlashingCtrl(LiveActor *pActor, bool a3) : NameObj("点滅")
{
    mActor = pActor;
    mToggleDraw = a3;
    mIsEnded = true;
    mOverrideInterval = 0;
    mTimer = 0;
    mFlashStartTime = 0;
    MR::connectToScene(this, 0x22, -1, -1, -1);
}

void FlashingCtrl::movement()
{
    if (!mIsEnded)
    {
        s32 curTimer = mTimer;
        mTimer = curTimer -1;
        if (curTimer - 1 <= 0 || MR::isClipped(mActor) || MR::isDead(mActor))
        {
            end();
        }
        else if (MR::isDemoActive())
        {
            if (mToggleDraw && MR::isNoEntryDrawBuffer(mActor))
            {
                MR::onEntryDrawBuffer(mActor);
            }
        }
        else if (isNowFlashing())
        {
            updateFlashing();
        }
    }
}

void FlashingCtrl::start(int timer)
{
    mIsEnded = false;
    mTimer = timer;
    mFlashStartTime = 180;
    mOverrideInterval = 0;
}

void FlashingCtrl::end()
{
    bool toggle = mToggleDraw;
    mTimer = 0;
    mIsEnded = true;

    if (toggle && !MR::isDead(mActor) && !MR::isClipped(mActor) && MR::isNoEntryDrawBuffer(mActor))
    {
        MR::onEntryDrawBuffer(mActor);
    }
}

s32 FlashingCtrl::getCurrentInterval() const
{
    if (mOverrideInterval)
    {
        return 8;
    }
    s32 curTimer = mTimer;
    s32 ret = 10;

    if (curTimer < 90)
    {
        ret = 5;
    }
    return ret;
}

bool FlashingCtrl::isNowFlashing() const
{
    return mTimer <= mFlashStartTime;
}

// TODO: I think it matches logically but code wise is a no 
bool FlashingCtrl::isNowOn() const
{
    return ((mTimer / getCurrentInterval()) | 0) % 2  == 0;
}

void FlashingCtrl::updateFlashing()
{
    if (isNowOn())
    {
        MR::offEntryDrawBuffer(mActor);
    }
    else
    {
        MR::onEntryDrawBuffer(mActor);
    }
}