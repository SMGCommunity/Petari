#include "Game/MapObj/MechaKoopaPartsHead.h"

MechaKoopaPartsHead::MechaKoopaPartsHead(const char *pName) : MapObjActor(pName) {

}

void MechaKoopaPartsHead::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo initInfo;
    initInfo.setupHioNode("地形オブジェ");
    initInfo.setupDefaultPos();
    initInfo.setupConnectToScene();
    initInfo.setupEffect(nullptr);
    initInfo.setupSound(6);
    initInfo.setupNerve(&NrvMechaKoopaPartsHead::MechaKoopaPartsHeadNrvWait::sInstance);
    initialize(rIter, initInfo);
    MR::declarePowerStar(this);
    MR::registerDemoActionNerve(this, &NrvMechaKoopaPartsHead::MechaKoopaPartsHeadNrvDemoBreak::sInstance, sDemoPartNameBreak);
    MR::registerDemoActionNerve(this, &NrvMechaKoopaPartsHead::MechaKoopaPartsHeadNrvDemoWhiteFadeOut::sInstance, sDemoPartNameFadeOut);
    MR::registerDemoActionNerve(this, &NrvMechaKoopaPartsHead::MechaKoopaPartsHeadNrvDemoWhiteWait::sInstance, sDemoPartNameWhite);
    MR::registerDemoActionNerve(this, &NrvMechaKoopaPartsHead::MechaKoopaPartsHeadNrvDemoWhiteFadeIn::sInstance, sDemoPartNameFadeIn);

    if (MR::isValidSwitchA(this)) {
        MR::onSwitchA(this);
    }
}

void MechaKoopaPartsHead::exeDemoBreak() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_BM_MECHA_KOOPA_DOWN", -1, -1);
        MR::moveVolumeStageBGM(0.0f, 0x1E);
        MR::shakeCameraInfinity(this, 0.2f, 2.0f);
    }

    MR::tryRumblePadMiddle(this, 0);
    MR::startLevelSound(this, "SE_BM_LV_MECHA_KOOPA_PRE_BREAK", -1, -1, -1);

    if (MR::isStep(this, 120)) {
        MR::emitEffect(this, "Explosion1");
        MR::emitEffect(this, "Explosion2");
        MR::emitEffect(this, "ExplosionEye1");
        MR::emitEffect(this, "ExplosionEye2");
        MR::startSound(this, "SE_OJ_M_KOOPA_EYE_BREAK", -1, -1);
    }

    if (MR::isStep(this, 300)) {
        MR::emitEffect(this, "Explosion");
        MR::startSound(this, "SE_BM_MECHA_KOOPA_BREAK_FIRE_ST", -1, -1);
    }
    
    if (MR::isGreaterStep(this, 300)) {
        MR::startLevelSound(this, "SE_BM_LV_MECHA_KOOPA_BREAK_FIRE", -1, -1, -1);
    }
}

void MechaKoopaPartsHead::exeDemoWhiteFadeOut() {
    if (MR::isFirstStep(this)) {
        MR::stopShakingCamera(this);
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraStrong();
        MR::startSound(this, "SE_BM_MECHA_KOOPA_EXPLODE", -1, -1);
        MR::closeWipeWhiteFade(60);
    }
}

void MechaKoopaPartsHead::exeDemoWhiteWait() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);

        if (MR::isValidSwitchA(this)) {
            MR::offSwitchA(this);
        }
        
        MR::setPlayerPos("爆破デモ後マリオ");
        MR::setPlayerStateWait();
    }
}

void MechaKoopaPartsHead::exeDemoWhiteFadeIn() {
    if (MR::isFirstStep(this)) {
        MR::openWipeWhiteFade(60);
    }

    if (MR::isDemoPartLastStep(sDemoPartNameFadeIn)) {
        setNerve(&NrvMechaKoopaPartsHead::MechaKoopaPartsHeadNrvDemoAppearStar::sInstance);
    }
}

void MechaKoopaPartsHead::exeDemoAppearStar() {
    if (MR::isFirstStep(this)) {
        MR::startAfterBossBGM();
        MR::requestAppearPowerStar(this);
    }

    if (MR::isEndPowerStarAppearDemo(this)) {
        kill();
    }
}

MechaKoopaPartsHead::~MechaKoopaPartsHead() {

}

namespace NrvMechaKoopaPartsHead {
    INIT_NERVE(MechaKoopaPartsHeadNrvWait);
    INIT_NERVE(MechaKoopaPartsHeadNrvDemoBreak);
    INIT_NERVE(MechaKoopaPartsHeadNrvDemoWhiteFadeOut);
    INIT_NERVE(MechaKoopaPartsHeadNrvDemoWhiteWait);
    INIT_NERVE(MechaKoopaPartsHeadNrvDemoWhiteFadeIn);
    INIT_NERVE(MechaKoopaPartsHeadNrvDemoAppearStar);

    void MechaKoopaPartsHeadNrvDemoAppearStar::execute(Spine *pSpine) const {
        MechaKoopaPartsHead* head = reinterpret_cast<MechaKoopaPartsHead*>(pSpine->mExecutor);
        head->exeDemoAppearStar();
    }

    void MechaKoopaPartsHeadNrvDemoWhiteFadeIn::execute(Spine *pSpine) const {
        MechaKoopaPartsHead* head = reinterpret_cast<MechaKoopaPartsHead*>(pSpine->mExecutor);
        head->exeDemoWhiteFadeIn();
    }

    void MechaKoopaPartsHeadNrvDemoWhiteWait::execute(Spine *pSpine) const {
        MechaKoopaPartsHead* head = reinterpret_cast<MechaKoopaPartsHead*>(pSpine->mExecutor);
        head->exeDemoWhiteWait();
    }

    void MechaKoopaPartsHeadNrvDemoWhiteFadeOut::execute(Spine *pSpine) const {
        MechaKoopaPartsHead* head = reinterpret_cast<MechaKoopaPartsHead*>(pSpine->mExecutor);
        head->exeDemoWhiteFadeOut();
    }

    void MechaKoopaPartsHeadNrvDemoBreak::execute(Spine *pSpine) const {
        MechaKoopaPartsHead* head = reinterpret_cast<MechaKoopaPartsHead*>(pSpine->mExecutor);
        head->exeDemoBreak();
    }

    void MechaKoopaPartsHeadNrvWait::execute(Spine *pSpine) const {

    }
};

void MechaKoopaPartsHead::initCaseNoUseSwitchB(const MapObjActorInitInfo &) {

}

void MechaKoopaPartsHead::initCaseUseSwitchB(const MapObjActorInitInfo &) {

}

void MechaKoopaPartsHead::initCaseNoUseSwitchA(const MapObjActorInitInfo &) {

}

void MechaKoopaPartsHead::initCaseUseSwitchA(const MapObjActorInitInfo &) {

}