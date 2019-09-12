#include "Animation/AnmPlayer.h"
#include "MR/StringUtil.h"

void AnmPlayerBase::update()
{
    if (this->mJ3DAnmBase == 0)
    {
        return;
    }

    this->mFrameCtrl.update();
}

void AnmPlayerBase::reflectFrame()
{
    if (this->mJ3DAnmBase == 0)
    {
        return;
    }
    
    this->mJ3DAnmBase->_8 = this->mFrameCtrl._10;
}

void AnmPlayerBase::start(const char *anmName)
{
    J3DAnmBase* res = (J3DAnmBase*)this->mResourceTable->getRes(anmName);

    if (res != this->mJ3DAnmBase)
    {
        this->changeAnimation(res);
        this->mJ3DAnmBase = res;
    }

    this->mFrameCtrl.init(this->mJ3DAnmBase->_6);
    this->mFrameCtrl._4 = this->mJ3DAnmBase->_4;
    this->mFrameCtrl._10 = 1.0f;
    this->mFrameCtrl._C = 0.0f;
}

void AnmPlayerBase::stop()
{
    this->stopAnimation();

    this->mFrameCtrl._C = 0.0f;
}

bool AnmPlayerBase::isPlaying(const char *anmName) const
{
    if (this->mJ3DAnmBase != 0)
    {
        const char* name = this->mResourceTable->getResName(this->mJ3DAnmBase);

        if (MR::isEqualStringCase(anmName, name) != 0)
        {
            return true;
        }
    }

    return false;
}