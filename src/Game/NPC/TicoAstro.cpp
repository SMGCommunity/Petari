#include "Game/NPC/TicoAstro.hpp"
#include "Game/Util/EventUtil.hpp"

TicoAstro::TicoAstro(const char* pName)
    : Tico(pName) {
}

void TicoAstro::init(const JMapInfoIter& rIter) {
    Tico::init(rIter);
    s32 arg2 = -1;
    MR::getJMapInfoArg2NoInit(rIter, &arg2);

    switch (arg2) {
    case 0:
        if (MR::calcCurrentGreenStarNum() <= 0) {
            makeActorDead();
        }
        break;
    case 1:
        if (MR::calcCurrentGreenStarNum() > 1) {
            return;
        }

        makeActorDead();
        break;
    case 2:
        if (MR::calcCurrentGreenStarNum() <= 2) {
            makeActorDead();
        }
        break;
    }
}

TicoAstro::~TicoAstro() {
}
