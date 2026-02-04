#include "Game/Enemy/Snakehead.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/Util/RailUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace {
    struct SnakeheadData {
        s32* _0;
        s32* _4;
        f32* _8;
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
        {&sStepForWaitBig, &sStepForRestBig, &sGoHomeSpeedNormal, "StraightAppear", "StraightWait", "StraightForward", "StraightForwardSmoke",
         "StraightBack"},
        {&sStepForWaitSmall, &sStepForRestSmall, &sGoHomeSpeedNormal, "StraightAppear", "Wait", "StraightForward", nullptr, "StraightBack"},
        {&sStepForWaitBigRace, &sStepForRestBigRace, &sGoHomeSpeedRace, nullptr, "Wait", "Forward", "ForwardSmoke", "Back"},
        {&sStepForWaitSmallRace, &sStepForRestSmallRace, &sGoHomeSpeedRace, nullptr, "Wait", "Forward", nullptr, "Back"}};
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

Snakehead::Snakehead(const char* pName) : LiveActor(pName) {
    mController = nullptr;
    mStarPointer = nullptr;
    _C4.x = 0.0f;
    _C4.y = 0.0f;
    _C4.z = 0.0f;
    _D0 = 15.0f;
    _D4 = 10.0f;
    _D8 = 1000.0f;
    _DC.x = 0.0f;
    _DC.y = 0.0f;
    _DC.z = 0.0f;
    _E8 = 0;
    mItem = nullptr;
    _94.identity();
}

void Snakehead::initAfterPlacement() {
    TVec3f headPos;
    MR::copyJointPos(this, "Head", &headPos);
    JMathInlineVEC::PSVECSubtract(&headPos, MR::getRailPointPosStart(this), &headPos);
    JMathInlineVEC::PSVECAdd(&headPos, MR::getRailPointPosEnd(this), &headPos);
    TVec3f v7;
    JMAVECScaleAdd(&mGravity, &headPos, &v7, -50.0f);
    TVec3f v6(mGravity);
    v6.scale(1000.0f);
    MR::getFirstPolyOnLineToMapExceptSensor(&_C4, nullptr, v7, v6, getSensor("body"));
    _94.set(MR::getJointMtx(this, "Body04"));
}

void Snakehead::kill() {
    MR::startSound(this, "SE_EM_EXPLODE_UNDER_WATER", -1, -1);
    MR::emitEffect(this, "Death");

    bool v3 = true;
    s32 val = _E8;

    if (val != 1) {
        if (val != 3) {
            v3 = false;
        }
    }

    if (v3) {
        TVec3f headPos;
        MR::copyJointPos(this, "Head", &headPos);
        MR::appearStarPiece(this, headPos, 9, 10.0f, 40.0f, 0);
        MR::startSound(this, "SE_OJ_STAR_PIECE_FIRST_W", -1, -1);
    } else {
        TVec3f attPos;
        MR::copyJointPos(this, "Antenna", &attPos);
        TPos3f itemMtx;
        itemMtx.set(getBaseMtx());
        itemMtx.setTrans(attPos);
        MR::appearKinokoOneUpPop(mItem, itemMtx, 15.0f);
    }

    LiveActor::kill();
}

void Snakehead::control() {
    mController->updateNerve();
    tryDPDSwoon();
    TVec3f jointPos;
    MR::copyJointPos(this, "Body04", &jointPos);
    _94.setTrans(jointPos);

    bool v3 = true;
    s32 val = _E8;

    if (val != 1) {
        if (val != 3) {
            v3 = false;
        }
    }

    if (!v3) {
        TVec3f body01Pos;
        MR::copyJointPos(this, "Body01", &body01Pos);
        TVec3f v6;
        f32 dist = PSVECDistance(&jointPos, &body01Pos);
        JMathInlineVEC::PSVECAdd(&jointPos, &body01Pos, &v6);
        v6.scale(0.5f);
        MR::setShadowDropPosition(this, "Body", v6);
        TVec3f v5;
        v5.x = 480.0f;
        v5.y = 300.0f;
        v5.z = dist;
        MR::setShadowVolumeBoxSize(this, "Body", v5);
    }
}

void Snakehead::calcAndSetBaseMtx() {
    TVec3f endPntDir;
    MR::calcRailEndPointDirection(&endPntDir, this);
    TVec3f stack_14;
    TVec3f stack_8;
    JMathInlineVEC::PSVECNegate(&mGravity, &stack_8);
    TPos3f frontUp;
    MR::makeMtxFrontUpPos(&frontUp, endPntDir, stack_8, mPosition);
    MR::setBaseTRMtx(this, frontUp);
    JMathInlineVEC::PSVECMultiply(mController->_C, mScale, &stack_14);
    MR::setBaseScale(this, stack_14);
}

void Snakehead::exeWaylay() {
    if (MR::isFirstStep(this)) {
        choiceAndStartBck("Waylay");
        MR::setBckFrameAndStop(this, 0.0f);
        _94.set(MR::getJointMtx(this, "Body04"));
    }

    if (isNearPlayerFromRail() && MR::getBckFrame(this) == 0.0f) {
        choiceAndStartBck("Waylay");
        MR::setBckFrame(this, 1.0f);
        MR::showModel(this);
    }

    if (MR::getBckFrame(this) == 2.0f) {
        MR::validateShadow(this, nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvSnakehead::SnakeheadNrvMoveForward::sInstance);
    }
}

void Snakehead::exeWait() {
    if (MR::isFirstStep(this)) {
        choiceAndStartBck("Wait");
    }

    if (MR::isGreaterEqualStep(this, *sSnakeheadDataTable[_E8]._0)) {
        bool v3 = true;
        s32 val = _E8;

        if (val != 3) {
            if (val != 2) {
                v3 = false;
            }
        }

        if (v3 || isNearPlayerFromRail()) {
            setNerve(&NrvSnakehead::SnakeheadNrvMoveForward::sInstance);
        }
    }
}

void Snakehead::exeMoveForward() {
    if (MR::isFirstStep(this)) {
        choiceAndStartBck("Forward");
        MR::setRailDirectionToEnd(this);
        MR::startSound(this, "SE_EM_SNAKEHEAD_FORWARD", -1, -1);
    }

    bool v3 = true;
    s32 val = _E8;

    if (val != 3) {
        if (val != 2) {
            v3 = false;
        }
    }

    if (!v3 && MR::isRailReachedNearGoal(this, 300.0f)) {
        MR::moveCoordAndFollowTrans(this, (_D0 / 1.5f));

    } else {
        MR::moveCoordAndFollowTrans(this, _D0);
    }

    if (MR::isRailReachedGoal(this)) {
        setNerve(&NrvSnakehead::SnakeheadNrvRest::sInstance);
    }
}

void Snakehead::exeRest() {
    if (MR::isStep(this, *sSnakeheadDataTable[_E8]._4)) {
        setNerve(&NrvSnakehead::SnakeheadNrvMoveBack::sInstance);
    }
}

void Snakehead::exeMoveBack() {
    if (MR::isFirstStep(this)) {
        choiceAndStartBck("Back");
        MR::setRailDirectionToStart(this);
        MR::startSound(this, "SE_EM_SNAKEHEAD_BACKWARD", -1, -1);
    }

    MR::moveCoordAndFollowTrans(this, _D4);

    if (MR::isRailReachedNearGoal(this, 300.0f)) {
        MR::setRailCoord(this, 300.0f);
        MR::moveTransToCurrentRailPos(this);
        setNerve(&NrvSnakehead::SnakeheadNrvWait::sInstance);
    }
}

void Snakehead::exeTurtleDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Damage", nullptr);
        MR::setRailDirectionToStart(this);
        MR::startSound(this, "SE_EV_SNAKEHEAD_DAMAGE", -1, -1);
    }

    f32 coord = *sSnakeheadDataTable[_E8]._8;

    if (MR::isRailReachedNearGoal(this, (300.0f + coord))) {
        MR::setRailCoord(this, 300.0f);
        MR::moveTransToCurrentRailPos(this);
    } else {
        MR::moveCoordAndFollowTrans(this, coord);
    }

    if (MR::isBckStopped(this)) {
        kill();
    }
}

void Snakehead::choiceAndStartBck(const char* pBck) {
    const char* v1 = nullptr;
    if (MR::isEqualString(pBck, "Forward")) {
        v1 = sSnakeheadDataTable[_E8]._14;
    } else if (MR::isEqualString(pBck, "Back")) {
        v1 = sSnakeheadDataTable[_E8]._1C;
    } else if (MR::isEqualString(pBck, "Waylay")) {
        v1 = sSnakeheadDataTable[_E8]._C;
    } else if (MR::isEqualString(pBck, "Wait")) {
        v1 = sSnakeheadDataTable[_E8]._10;
    }

    if (v1 != nullptr) {
        if (!MR::isEqualString(pBck, "Waylay")) {
            MR::startBck(this, v1, nullptr);

        } else {
            MR::startBckNoInterpole(this, v1);
        }

        if (MR::isExistBtk(this, v1)) {
            MR::startBtk(this, v1);
        } else if (MR::isExistBtk(this, "Wait")) {
            MR::startBtk(this, "Wait");
            MR::setBtkFrameAndStop(this, 0.0f);
        }
    }
}

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
