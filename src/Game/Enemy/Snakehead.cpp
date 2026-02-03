#include "Game/Enemy/Snakehead.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"

namespace {
    struct SnakeheadData {
        s32 _0;
        s32 _4;
        f32 _8;
        const char* _C;
        const char* _10;
        const char* _14;
        const char* _18;
        const char* _1C;
    };

    static s32 sStepForWaitBig = 0x1E;
    static s32 sStepForRestBig = 0x64;
    static s32 sStepForWaitSmall = 0x3C;
    static s32 sStepForRestSmall = 0x78;
    static f32 sGoHomeSpeedNormal = 20.0f;
    static s32 sStepForWaitBigRace = 0x1E;
    static s32 sStepForRestBigRace = 0x64;
    static s32 sStepForWaitSmallRace = 0x3C;
    static s32 sStepForRestSmallRace = 0x78;
    static f32 sGoHomeSpeedRace = 50.0f;

    static SnakeheadData sSnakeheadDataTable[] = {
        {sStepForWaitBig, sStepForRestBig, sGoHomeSpeedNormal, "StraightAppear", "StraightWait", "StraightForward", "StraightForwardSmoke",
         "StraightBack"},
        {sStepForWaitSmall, sStepForRestSmall, sGoHomeSpeedNormal, "StraightAppear", "Wait", "StraightForward", nullptr, "StraightBack"},
        {sStepForWaitBigRace, sStepForRestBigRace, sGoHomeSpeedRace, nullptr, "Wait", "Forward", "ForwardSmoke", "Back"},
        {sStepForWaitSmallRace, sStepForRestSmallRace, sGoHomeSpeedRace, nullptr, "Wait", "Forward", nullptr, "Back"}};
};  // namespace

namespace NrvSnakehead {
    NEW_NERVE(SnakeheadNrvWaylay, Snakehead, Waylay);
    NEW_NERVE(SnakeheadNrvWait, Snakehead, Wait);
    NEW_NERVE(SnakeheadNrvMoveForward, Snakehead, MoveForward);
    NEW_NERVE(SnakeheadNrvRest, Snakehead, Rest);
    NEW_NERVE(SnakeheadNrvMoveBack, Snakehead, MoveBack);
    NEW_NERVE(SnakeheadNrvTurtleDown, Snakehead, TurtleDown);
    NEW_NERVE_ONEND(SnakeheadNrvDPDSwoon, Snakehead, DPDSwoon, DPDSwoon);
};  // namespace NrvSnakehead

bool Snakehead::tryDPDSwoon() {
    if (isNerve(&NrvSnakehead::SnakeheadNrvDPDSwoon::sInstance)) {
        return false;
    }

    if (isNerve(&NrvSnakehead::SnakeheadNrvWaylay::sInstance)) {
        return false;
    }

    if (isNerve(&NrvSnakehead::SnakeheadNrvTurtleDown::sInstance)) {
        return false;
    }

    if (!mStarPointer->tryStartPointBind()) {
        return false;
    }

    setNerve(&NrvSnakehead::SnakeheadNrvDPDSwoon::sInstance);
    return true;
}

bool Snakehead::isNearPlayerFromRail() const {
    TVec3f nearestPos;
    MR::calcNearestRailPos(&nearestPos, this, *MR::getPlayerPos());
    return PSVECDistance(&nearestPos, *MR::getPlayerPos()) <= _D8;
}

Snakehead::~Snakehead() {
}

void Snakehead::endDPDSwoon() {
    mStarPointer->kill();
}

void Snakehead::exeDPDSwoon() {
    MR::updateActorStateAndNextNerve(this, mStarPointer, &NrvSnakehead::SnakeheadNrvMoveBack::sInstance);
}
