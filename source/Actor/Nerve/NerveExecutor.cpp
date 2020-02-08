#include "Actor/Nerve/NerveExecutor.h"

extern int __cntlzw (int);

NerveExecutor::NerveExecutor(const char* pName) : mSpine(0) { }

void NerveExecutor::initNerve(const Nerve* pNerve)
{
    mSpine = new Spine(this, pNerve);
}

void NerveExecutor::updateNerve()
{
    if (mSpine != 0)
    {
        mSpine->update();
    }
}

void NerveExecutor::setNerve(const Nerve* pNerve)
{
    mSpine->setNerve(pNerve);
}

/*bool NerveExecutor::isNerve(const Nerve* pNerve) const
{
    const Nerve* nerve = mSpine->getCurrentNerve();
    // todo -- this isn't properly assembled
    return __cntlzw(nerve - pNerve) >> 5;
}*/

u32 NerveExecutor::getNerveStep() const
{
    return mSpine->mNerveStep;
}