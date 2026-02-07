#include "Game/Boss/KoopaSequencer.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaDemoPowerUp.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "revolution/types.h"

KoopaSequencer::KoopaSequencer(const char* pName, s32 pNum)
    : NerveExecutor(pName), mParent(nullptr), mDemoPowerUp(nullptr), _10(0), _14(0), _18(1), _1C(pNum), _20(false) {}

void KoopaSequencer::init(Koopa* pParent, const JMapInfoIter& pIter) {
    mParent = pParent;
    mDemoPowerUp = new KoopaDemoPowerUp(mParent);
    mParent->init(pIter);
}
