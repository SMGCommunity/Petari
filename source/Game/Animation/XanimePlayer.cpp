#include "Game/Animation/XanimePlayer.hpp"

void XanimePlayer::setDefaultAnimation(const char *name)
{
    const XanimeGroupInfo *defaultAnimation = mResourceTable->getGroupInfo(name);
    if (mCurrentAnimation == mDefaultAnimation) {
        changeAnimation(defaultAnimation);
    }
    mDefaultAnimation = defaultAnimation;
    _7D = true;
}

void XanimePlayer::changeCurrentAnimation(const XanimeGroupInfo *animation)
{
    if (mCurrentAnimation == animation) {
        return;
    }
    mPrevAnimation = mCurrentAnimation;
    mCurrentAnimation = animation;
    updateAfterMovement();
}
