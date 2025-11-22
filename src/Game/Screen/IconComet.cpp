#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/IconComet.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"

namespace NrvIconComet {
    NEW_NERVE(IconCometNrvWait, IconComet, Wait);
};

IconComet::IconComet()
    : LayoutActor("コメットアイコン", true) {
}

void IconComet::init(const JMapInfoIter& rIter) {
    initLayoutManager("IconComet", 1);
    initNerve(&NrvIconComet::IconCometNrvWait::sInstance);
}

bool IconComet::appearIfLanding(const char* pStageName) {
    if (!MR::isGalaxyCometLandInStage(pStageName)) {
        return false;
    }

    MR::setCometAnimFromId(this, MR::getEncounterGalaxyCometNameId(pStageName), 0);
    LayoutActor::appear();

    return true;
}

void IconComet::appearByCometNameId(int id) {
    MR::setCometAnimFromId(this, id, 0);
    LayoutActor::appear();
}

void IconComet::exeWait() {
}
