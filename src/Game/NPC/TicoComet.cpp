#include "Game/NPC/TicoComet.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/StarPieceDirector.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/Screen/GalaxyMapController.hpp"
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

namespace NrvTicoComet {
    NEW_NERVE(TicoCometNrvDemoAnim, TicoComet, DemoAnim);
    NEW_NERVE(TicoCometNrvDemoFade, TicoComet, DemoFade);
    NEW_NERVE(TicoCometNrvDemoEnd, TicoComet, DemoEnd);
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
    mParam._14 = "Wait";
    mParam._18 = "Wait";
    mParam._1C = "Talk";
    mParam._20 = "Talk";
    mParam._4 = 3000.0f;
    setDefaults();
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
    TVec3f dir = MR::getCamZdir();
    
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
        mParam._1C = "Joy2";
        mParam._20 = "Joy2";
        popAndPushNerve(&NrvTicoEat::TicoEatNrvEatEnd::sInstance);
    }
}

TicoComet::TicoComet(const char *pName) : TicoEat(pName) {
    
}

TicoEat::~TicoEat() {
    
}

void TicoComet::init(const JMapInfoIter &rIter) {
    NPCActorCaps caps("TicoComet");
    NPCActorItem item;
    item.mActor = "TicoComet";
    item._4 = "";
    item._8 = "";
    item.mArchive = "";
    item._10 = "";
    caps.setDefault();
    caps.mSensorSize = 100.0f;
    caps.mSensorOffset.y = 100.0f;
    caps.mUseShadow = true;
    caps._30 = 6;
    caps._58 = 2;
    caps._44 = "Center";
    caps._6C = "Center";
    caps._38 = 0;
    initialize(rIter, caps);
    MR::addHitSensorAtJointEye(this, "Mouth", "Mouth", 8, 30.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::getNPCItemData(&item, 0);
    equipment(item, true);
    MR::startAction(_94, "LeftRotate");
    MR::startAction(_98, "RightRotate");
    MR::startBtk(this, "TicoComet");
    _194 = 20;
    _198 = 20;
    MR::declareStarPieceReceiver(this, 20);
    MR::setMessageArg(mMsgCtrl, _194);
    MR::registerBranchFunc(mMsgCtrl, TalkMessageFunc(this, &TicoComet::branchFunc));
    MR::registerEventFunc(mMsgCtrl, TalkMessageFunc(this, &TicoComet::eventFunc));
    MR::setDistanceToTalk(mMsgCtrl, 350.0f);
    if (!MR::isRosettaTalkTrickComet()) {
        makeActorDead();
    }

    AstroDemoFunction::tryRegisterSimpleCastIfAstroGalaxy(this);
    MR::startBva(this, "Small0");
    MR::startBrk(this, "Normal");
    MR::setBrkFrameAndStop(this, 0.0f);
    TicoEat::init(rIter);
    _17C.set(0xC8, 0, 0xFF, 0xFF);
}

bool TicoComet::branchFunc(u32 val) {
    return MR::getStarPieceNum() >= _194;
}

bool TicoComet::tryDemo() {
    if (isEmptyNerve()) {
        pushNerve(&NrvTicoComet::TicoCometNrvDemoAnim::sInstance);
    }

    if (isNerve(&NrvTicoComet::TicoCometNrvDemoEnd::sInstance)) {
        popNerve();
        return true;
    }

    return false;
}

bool TicoComet::eventFunc(u32 event) {
    if (event < 3) {
        return TicoEat::eventFunc(event);
    }

    if (event == 3) {
        return tryDemo();
    }

    return true;
}

void TicoComet::startReactionSound() {
    if (_D8) {
        MR::startSound(this, "SE_SM_NPC_TRAMPLED", -1, -1);
        MR::startSound(this, "SE_SV_TICOCOMET_TRAMPLED", -1, -1);
    }

    if (isPointingSe()) {
        MR::startDPDHitSound();
        MR::startSound(this, "SE_SV_TICOCOMET_POINT", -1, -1);
    }

    if (_D9) {
        MR::startSound(this, "SE_SM_TICOCOMET_SPIN", -1, -1);
    }

    if (_DB) {
        MR::startSound(this, "SE_SV_TICOCOMET_STAR_PIECE_HIT", -1, -1);
    }
}

void TicoComet::exeDemoAnim() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Demo");
        MR::startSound(this, "SE_SM_TICOFAT_META_ITEM", -1, -1);
        MR::startSound(this, "SE_SM_TICOFAT_META", -1, -1);
    }

    if (MR::isGreaterEqualStep(this, 90)) {
        MR::startSound(this, "SE_SM_METAMORPHOSE_SMOKE", -1, -1);
        MR::startSystemSE("SE_DM_TICOFAT_MORPH_WIPE_IN", -1, -1);
        popAndPushNerve(&NrvTicoComet::TicoCometNrvDemoFade::sInstance);
    }
}

void TicoComet::exeDemoFade() {
    if (MR::isFirstStep(this)) {
        MR::closeWipeWhiteFade(30);
        MR::startSystemSE("SE_DM_TICOFAT_MORPH_WIPE_OUT", -1, -1);
    }

    if (!MR::isWipeActive()) {
        mParam._1C = "Talk";
        mParam._20 = "Talk";
        MR::forceOpenWipeWhiteFade();
        MR::forceToNextStateGalaxyCometScheduler();
        MR::startGalaxyMapLayoutForTicoComet();
        MR::startBva(this, "Small0");
        MR::startAction(this, "Wait");
        MR::startBckNoInterpole(this, "Wait");
        MR::clearGotCountStarPieceReceiver(this);
        _198 = _194;
        popAndPushNerve(&NrvTicoComet::TicoCometNrvDemoEnd::sInstance);
    }
}

void TicoEat::exeEatEnd() {
    
}

void TicoComet::exeDemoEnd() {

}

void TicoEat::startReactionSound() {

}

TicoComet::~TicoComet() {
    
}
