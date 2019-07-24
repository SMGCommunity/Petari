#include "Animation/AnmPlayer.h"
#include "MR/StringUtil.h"

void AnmPlayerBase::update()
{
    if (this->mSrc == 0)
    {
        return;
    }

    this->mFrameCtrl.update();
}

void AnmPlayerBase::stop()
{
    this->stopAnimation();

    this->mFrameCtrl._C = AnmPlayerBase::zero;
}

bool AnmPlayerBase::isPlaying(const char *anmName) const
{
    if (this->mSrc != 0)
    {
        const char* name = this->mResourceTable->getResName(this->mSrc);

        if (MR::isEqualStringCase(anmName, name) != 0)
        {
            return true;
        }
    }

    return false;
}