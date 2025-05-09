#include "Game/NPC/TicoComet.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/StarPieceDirector.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace NrvTicoEat {
    NEW_NERVE(TicoEatNrvEatPre, TicoEat, EatPre);
    NEW_NERVE(TicoEatNrvEatNow, TicoEat, EatNow);
    NEW_NERVE(TicoEatNrvEatPst, TicoEat, EatPst);
    NEW_NERVE(TicoEatNrvEatEnd, TicoEat, EatEnd);
    NEW_NERVE(TicoEatNrvReaction, TicoEat, Reaction);
};

namespace {
    class TicoEatParam : public AnimScaleParam {
    public:
        TicoEatParam();
    };

    TicoEatParam::TicoEatParam() : AnimScaleParam() {
        _20 = 0.12f;
        _24 = 0.12f;
        _28 = 0.89999998f;
    }

    static TicoEatParam sParam;
};

TicoEat::TicoEat(const char *pName) : Tico(pName) {
    _190 = 0;
    _194 = 0;
    _198 = 0;
}

void TicoEat::init(const JMapInfoIter &rIter) {
    mScaleController = new AnimScaleController(&sParam);
    mReactionNerve = &NrvTicoEat::TicoEatNrvReaction::sInstance;
    _178 = MR::getJointMtx(this, "Center");
    _FC = "Wait";
    _100 = "Wait";
    _104 = "Talk";
    _108 = "Talk";
    _EC = 3000.0f;
    _130 = "Spin";
    _134 = "Trampled";
    _138 = "Pointing";
    _13C = "Reaction";
    _12C = 1000.0f;
}

void TicoEat::control() {
    if (MR::isBckPlaying(this, "Joy2")) {
        MR::startLevelSound(this, "SE_SM_LV_TICOFAT_GLAD", -1, -1, -1);
    }

    Tico::control();
}

bool TicoEat::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (MR::isMsgStarPieceGift(msg)) {
        if (_198 == _194) {
            MR::startAction(this, "Eat0");
            MR::startBva(this, "Big0");
        }

        f32 val = (0.66f * _194);
        if (_198 == (s32)val) {
            f32 frame = MR::getBckFrame(this);
            MR::startAction(this, "Eat1");
            MR::startBva(this, "Big1");
            MR::setBckFrame(this, frame);
            mScaleController->startHitReaction();
        }

        val = (0.33f * _194);
        if (_198 == (s32)val) {
            f32 frame = MR::getBckFrame(this);
            MR::startAction(this, "Eat2");
            MR::startBva(this, "Big2");
            MR::setBckFrame(this, frame);
            mScaleController->startHitReaction();
        }

        _198--;
        return true;
    }

    return Tico::receiveMsgPlayerAttack(msg, a2, a3);
}

void TicoEat::initStarPiece(s32 num) {
    _194 = num;
    _198 = num;
    MR::declareStarPieceReceiver(this, num);
}

bool TicoEat::tryEat() {
    if (isEmptyNerve()) {
        pushNerve(&NrvTicoEat::TicoEatNrvEatPre::sInstance);
    }
    
    if (isNerve(&NrvTicoEat::TicoEatNrvEatEnd::sInstance)) {
        popNerve();
        return true;
    }

    return false;
}

bool TicoEat::hasEnoughStarPiece() const {
    return MR::getStarPieceNum() >= _194;
}

bool TicoEat::eventFunc(u32 event) {
    if (event == 0) {
        return tryEat();
    }

    if (event == 1) {
        MR::forceAppearStarPieceCounterForTicoFat();
    }
    
    if (event == 2) {
        MR::disappearStarPieceCounterForTicoFat();
    }

    return true;
}

void TicoEat::exeReaction() {
    if (MR::isFirstStep(this) && (_D8 || _DB)) {
        mScaleController->startHitReaction();
    }

    startReactionSound();
    
    if (MR::tryStartReactionAndPopNerve(this)) {
        return;
    }
}

void TicoEat::exeEatPre() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Excite");
        MR::startBva(this, "Big0");
    }

    
    TVec3f neg;
    TVec3f dir = MR::getCamZDir();
    
    JMathInlineVEC::PSVECNegate(&dir, &neg);
    if (MR::turnQuatZDirRad(&_A0, _A0, neg, 0.034906585f)) {
        popAndPushNerve(&NrvTicoEat::TicoEatNrvEatNow::sInstance);
    }
}

void TicoEat::exeEatNow() {
    MR::isFirstStep(this);
    if (_198 && MR::isIntervalStep(this, 100/ _194)) {
        MR::giftStarPieceToTarget(getSensor("Mouth"), 1);
    }

    MR::startLevelSound(this, "SE_SM_LV_TICOFAT_ABSORB", -1, -1, -1);
    MR::startLevelSound(this, "SE_SM_LV_TICOFAT_EATING", -1, -1, -1);
    MR::startLevelSound(this, "SE_SM_LV_TICOFAT_EATING", 0, -1, -1);

    if (_198 == 0) {
        popAndPushNerve(&NrvTicoEat::TicoEatNrvEatPst::sInstance);
    }
}

void TicoEat::exeEatPst() {
    if (MR::isGreaterEqualStep(this, 4)) {
        _104 = "Joy2";
        _108 = "Joy2";
        popAndPushNerve(&NrvTicoEat::TicoEatNrvEatEnd::sInstance);
    }
}
