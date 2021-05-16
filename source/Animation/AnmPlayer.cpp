#include "Animation/AnmPlayer.h"
#include "Util/StringUtil.h"

void AnmPlayerBase::update()
{
    if (mJ3DAnmBase == 0)
    {
        return;
    }

    mFrameCtrl.update();
}

void AnmPlayerBase::reflectFrame()
{
    if (mJ3DAnmBase == 0)
    {
        return;
    }

    mJ3DAnmBase->_8 = mFrameCtrl._10;
}

void AnmPlayerBase::start(const char *pAnmName)
{
    J3DAnmBase* pRes = (J3DAnmBase*)mResourceTable->getRes(pAnmName);

    if (pRes != mJ3DAnmBase)
    {
        changeAnimation(pRes);
        mJ3DAnmBase = pRes;
    }

    mFrameCtrl.init(mJ3DAnmBase->_6);
    mFrameCtrl._4 = mJ3DAnmBase->_4;
    mFrameCtrl._10 = 1.0f;
    mFrameCtrl._C = 0.0f;
}

void AnmPlayerBase::stop()
{
    stopAnimation();

    mFrameCtrl._C = 0.0f;
}

bool AnmPlayerBase::isPlaying(const char *pAnmName) const
{
    if (mJ3DAnmBase != 0)
    {
        const char* pName = mResourceTable->getResName(mJ3DAnmBase);

        if (MR::isEqualStringCase(pAnmName, pName) != 0)
        {
            return true;
        }
    }

    return false;
}