#include "System/Galaxy/GalaxyCometState.h"

GalaxyCometState::GalaxyCometState() : NerveExecutor("GalaxyCometState")
{
    mNeedsUpdate = true;
    initNerve(GalaxyCometStateLand::sInstance);
}

void GalaxyCometState::update()
{
    if (mNeedsUpdate)
    {
        updateNerve();
    }
}

bool GalaxyCometState::isLand() const
{
    return isNerve(GalaxyCometStateLand::sInstance);
}

bool GalaxyCometState::isHide() const
{
    return isNerve(GalaxyCometStateHide::sInstance);
}

void GalaxyCometState::forceToNext()
{
    Nerve* nerve;

    // todo -- fix some registers not being included
    nerve = GalaxyCometStateLand::sInstance;

    if (isNerve(GalaxyCometStateLand::sInstance))
    {
        nerve = GalaxyCometStateHide::sInstance;
    }

    setNerve(nerve);
}

s32 GalaxyCometState::getStateIndex() const
{
    /* todo */
    return 0;
}

s32 GalaxyCometState::getPastSecond() const
{
    return 0;
}