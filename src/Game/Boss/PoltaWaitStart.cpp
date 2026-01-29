#include "Game/Boss/PoltaWaitStart.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaActionBase.hpp"
#include "Game/Boss/PoltaFunction.hpp"

PoltaWaitStart::PoltaWaitStart(Polta* pPolta) : PoltaActionBase("ポルタ開始待ち", pPolta) {}

void PoltaWaitStart::appear() {
  mIsDead = false;
  PoltaFunction::killLeftArm(mPoltaPtr);
  PoltaFunction::killRightArm(mPoltaPtr);
  MR::hideModel(mPoltaPtr);
}

PoltaWaitStart::~PoltaWaitStart() {}
