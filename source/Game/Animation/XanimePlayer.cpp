#include "Game/Animation/XanimePlayer.hpp"

void XanimePlayer::setDefaultAnimation(const char *name) {
    const XanimeGroupInfo *animation = mResourceTable->getGroupInfo(name);
    if(mStopAnimation == mDefaultAnimation) {
        changeAnimation(animation);
    }
    mDefaultAnimation = animation;
    _7D = true;
}
