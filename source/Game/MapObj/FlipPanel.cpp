#include "Game/MapObj/FlipPanel.h"
#include "Game/LiveActor/ModelObj.h"
#include "Game/LiveActor/LiveActorGroupArray.h"
#include "JSystem/JMath/JMath.h"
#include "Game/Util.h"

FlipPanel::FlipPanel(const char *pName) : MapObjActor(pName) {
    mDelegator = 0;
    mFlipPanelGroup = 0;
    _CC = 0;
    _CD = 0;
    _D0 = 0;
    mIsReverse = false;
}

void FlipPanel::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect("FlipPanel");
    info.setupSound(4);
    info.setupNerve(&NrvFlipPanel::FlipPanelNrvFront::sInstance);
    initialize(rIter, info);
    mIsReverse = isObjectName("FipPanelReverse");
    mDelegator = MR::createJointDelegatorWithNullChildFunc(this, &FlipPanel::calcJointMove, "Panel");
    mFlipPanelGroup = MR::joinToGroupArray(this, rIter, 0, 0x20);
    MR::invalidateClipping(this);
    MR::startBck(this, "PanelB", 0);
    f32 frameMax = MR::getBckFrameMax(this);
    MR::setBckFrameAndStop(this, frameMax);

    if (mIsReverse) {
        MapObjActorUtil::appearBloomModel(this);
    }
    else {
        MapObjActorUtil::killBloomModel(this); 
    }
}

void FlipPanel::exeFrontLand() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "PanelB", 0);

        if (mIsReverse) {
            MapObjActorUtil::appearBloomModel(this);
            ModelObj* bloomModel = mBloomModel;
            u32 frameMax = MR::getBrkFrameMax(bloomModel);
            u32 val = sBloomSyncStep / frameMax;
            frameMax = sBloomSyncStep - (val) * frameMax;
            MR::setBrkFrame(bloomModel, frameMax);
        }
        else {
            MapObjActorUtil::killBloomModel(this);
        }

        MR::tryRumblePadMiddle(this, 0);
        MR::startSound(this, "SE_OJ_FLIP_PANEL_CHANGE", -1, -1);

        if (_CD) {
            MR::sendMsgToGroupMember(0x67, this, getSensor(0), "body");
            _CD = 0;
        }
    }

    if (MR::isOnGroundPlayer() && !MR::isOnPlayer(this)) {
        setNerve(&NrvFlipPanel::FlipPanelNrvFront::sInstance);
    }
}

void FlipPanel::exeBackLand() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "PanelA", 0);

        if (mIsReverse) {
            MapObjActorUtil::killBloomModel(this);
        }
        else {
            MapObjActorUtil::appearBloomModel(this);
            ModelObj* bloomModel = mBloomModel;
            u32 frameMax = MR::getBrkFrameMax(bloomModel);
            u32 val = sBloomSyncStep / frameMax;
            frameMax = sBloomSyncStep - (val) * frameMax;
            MR::setBrkFrame(bloomModel, frameMax);
        }

        MR::tryRumblePadMiddle(this, 0);
        MR::startSound(this, "SE_OJ_FLIP_PANEL_CHANGE", -1, -1);

        if (!_CD) {
            MR::sendMsgToGroupMember(0x66, this, getSensor(0), "body");
            _CD = 1;
        }
    }

    if (MR::isOnGroundPlayer() && !MR::isOnPlayer(this)) {
        setNerve(&NrvFlipPanel::FlipPanelNrvBack::sInstance);
    }
}

void FlipPanel::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }

    if (_CC && !MR::isOnPlayer(this) && MR::isOnGroundPlayer()) {
        _CC = 0;
    }

    if (!_CC) {
        if (checkPlayerOnTop()) {
            if (isNerve(&NrvFlipPanel::FlipPanelNrvFront::sInstance)) {
                setNerve(&NrvFlipPanel::FlipPanelNrvBackLand::sInstance);
            }
            else {
                setNerve(&NrvFlipPanel::FlipPanelNrvFrontLand::sInstance);
            }
        }
    }
}

void FlipPanel::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_OJ_FLIP_PANEL_COMPLETE", -1, -1);
        MR::startBck(this, "PanelEnd", 0);
        MapObjActorUtil::killBloomModel(this);
        _CC = 0;
    }

    if (MR::isBckStopped(this)) {
        if (MR::isEffectValid(this, "FlipPanelGold")) {
            MR::validateClipping(this);
        }
    }
}

void FlipPanel::appear() {
    MapObjActor::appear();
    if (mIsReverse) {
        MapObjActorUtil::appearBloomModel(this);
    }
    else {
        MapObjActorUtil::killBloomModel(this);
    }
}

void FlipPanel::endClipped() {
    MapObjActor::endClipped();

    if (MR::getBrkCtrl(mBloomModel)) {
        ModelObj* bloomModel = mBloomModel;
        u32 frameMax = MR::getBrkFrameMax(bloomModel);
        u32 val = sBloomSyncStep / frameMax;
        frameMax = sBloomSyncStep - (val) * frameMax;
        MR::setBrkFrame(bloomModel, frameMax);
    }
}

void FlipPanel::calcAndSetBaseMtx() {
    MapObjActor::calcAndSetBaseMtx();

    if (_CC) {
        mDelegator->registerCallBack(); 
    }
}

bool FlipPanel::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (isNerve(&NrvFlipPanel::FlipPanelNrvEnd::sInstance)) {
        return 0;
    }

    if ((msg - 0x66) <= 1) {
        _CC = this == a2->mActor;
        return 1;
    }
    else {
        if (msg == 0x69) {
            MR::invalidateClipping(this);
            setNerve(&NrvFlipPanel::FlipPanelNrvEndPrepare::sInstance);
            return 1;
        }
        else {
            return 0;
        }
    }
}

bool FlipPanel::calcJointMove(TPos3f *pPos, const JointControllerInfo &rInfo) {
    if (!_CC) {
        return false;
    }

    TVec3f jointPos;
    MR::copyJointPos(this, "Panel", &jointPos);
    TVec3f upVec;
    MR::calcUpVec(&upVec, this);
    JMAVECScaleAdd((const Vec*)&upVec, (const Vec*)&jointPos, (Vec*)&jointPos, -25.0f);
    pPos->mMtx[0][3] = jointPos.x;
    pPos->mMtx[1][3] = jointPos.y;
    pPos->mMtx[2][3] = jointPos.z;
    return true;
}

bool FlipPanel::checkPlayerOnTop() {
    if (MR::isPlayerJumpRising() && _D0) {
        _D0 = 0;
        return true;
    }

    if (!MR::isOnPlayer(this)) {
        _D0 = 0;
        return false;
    }

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);

    if (!MR::getPlayerGroundingPolygon()) {
        return false;
    }

    TVec3f* groundNormal = MR::getPlayerGroundNormal();

    bool ret = JGeometry::TUtil<f32>::epsilonEquals(upVec.x, groundNormal->x, 0.0000038146973f)
        && JGeometry::TUtil<f32>::epsilonEquals(upVec.y, groundNormal->y, 0.0000038146973f)
        && JGeometry::TUtil<f32>::epsilonEquals(upVec.z, groundNormal->z, 0.0000038146973f);
    
    if (!ret) {
        _D0 = 0;
        return false;
    }

    if (_D0 < 2) {
        _D0 = _D0 + 1;
        return false;
    }

    _D0 = 0;
    return true;
}

FlipPanelObserver::FlipPanelObserver(const char *pName) : LiveActor(pName) {
    _8C = 0;
    _90 = 0;
    mDemoDelay = 0;
    mPowerStarId = -1;
    _9C = 0;
    sBloomSyncStep = 0;
}

void FlipPanelObserver::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::invalidateClipping(this);
    _8C = MR::joinToGroupArray(this, rIter, 0, 0x20);
    MR::useStageSwitchWriteA(this, rIter);
    initSound(2, false);
    MR::getJMapInfoArg7NoInit(rIter, &mPowerStarId);

    if (mPowerStarId != -1) {
        if (!mPowerStarId) {
            MR::declarePowerStar(this);
        }
        else {
            MR::declarePowerStar(this, mPowerStarId);
        }
    }

    MR::getJMapInfoArg0NoInit(rIter, &mDemoDelay);
    mDemoDelay += 0x28;

    if (MR::isEqualStageName("HeavensDoorGalaxy")) {
        _9C = 1;
    }
    else {
        _9C = 0;
    }

    initNerve(&NrvFlipPanelObserver::FlipPanelObserverNrvWait::sInstance);
    bool ret = MR::useStageSwitchReadAppear(this, rIter);

    if (ret) { 
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }
}

void FlipPanelObserver::exeWait() {
    if (MR::isFirstStep(this) && MR::isValidSwitchAppear(this)) {
        MR::callAppearAllGroupMember(this);
        MR::startSound(this, "SE_OJ_FLIP_PANEL_APPEAR", -1, -1);
    }

    if (_90 == _8C->mObjectCount - 1 && MR::tryStartDemo(this, "FlipPanelComplete")) {
        setNerve(&NrvFlipPanelObserver::FlipPanelObserverNrvComplete::sInstance);
    }
    else {
        sBloomSyncStep++;
    }
}

void FlipPanelObserver::exeComplete() {
    if (MR::isFirstStep(this)) {
        MR::callRequestMovementOnAllGroupMember(this);
        MR::requestMovementOn(_8C);
        MR::sendMsgToGroupMember(0x69, this, getSensor(0), "body");
    }

    if (MR::isGreaterStep(this, mDemoDelay)) {
        MR::endDemo(this, "FlipPanelComplete");

        if (mPowerStarId != -1) {
            if (!mPowerStarId) {
                MR::requestAppearPowerStar(this, mPosition);
            }
            else {
                MR::requestAppearPowerStar(this, mPowerStarId, mPosition);
            }
        }

        setNerve(&NrvFlipPanelObserver::FlipPanelObserverNrvDemoWait::sInstance);
    }
}

void FlipPanelObserver::exeDemoWait() {
    if (!MR::isDemoActive()) {
        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }

        makeActorDead();
    }
}

void FlipPanelObserver::initAfterPlacement() {
    if (MR::isValidSwitchAppear(this)) {
        MR::callMakeActorDeadAllGroupMember(this);
    }
}

bool FlipPanelObserver::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (msg == 0x66) {
        if (!_90) {
            if (_9C) {
                MR::startSystemME("ME_FLIP_PANEL_INV_OFF_FIRST");
            }
            else {
                MR::startSystemME("ME_FLIP_PANEL_ON_FIRST");
            }
        }
        else {
            if (_9C) {
                MR::startSystemME("ME_FLIP_PANEL_INV_OFF");
            }
            else {
                MR::startSystemME("ME_FLIP_PANEL_ON");
            }
        }

        _90++;
        return 1;
    }
    else if (msg == 0x67) {
        if (_9C) {
            MR::startSystemME("ME_FLIP_PANEL_INV_ON");
        }
        else {
            MR::startSystemME("ME_FLIP_PANEL_OFF");
        }

        _90--;
        return 1;
    }

    return 0;
}

namespace NrvFlipPanel {
    FlipPanelNrvFrontLand FlipPanelNrvFrontLand::sInstance;
    FlipPanelNrvBackLand FlipPanelNrvBackLand::sInstance;
    FlipPanelNrvFront FlipPanelNrvFront::sInstance;
    FlipPanelNrvBack FlipPanelNrvBack::sInstance;
    FlipPanelNrvEndPrepare FlipPanelNrvEndPrepare::sInstance;
    FlipPanelNrvEnd FlipPanelNrvEnd::sInstance;
};

namespace NrvFlipPanelObserver {
    FlipPanelObserverNrvWait FlipPanelObserverNrvWait::sInstance;
    FlipPanelObserverNrvComplete FlipPanelObserverNrvComplete::sInstance;
    FlipPanelObserverNrvDemoWait FlipPanelObserverNrvDemoWait::sInstance;
};

void NrvFlipPanelObserver::FlipPanelObserverNrvDemoWait::execute(Spine *pSpine) const {
    FlipPanelObserver* obs = reinterpret_cast<FlipPanelObserver*>(pSpine->mExecutor);
    obs->exeDemoWait();
}

void NrvFlipPanelObserver::FlipPanelObserverNrvComplete::execute(Spine *pSpine) const {
    FlipPanelObserver* obs = reinterpret_cast<FlipPanelObserver*>(pSpine->mExecutor);
    obs->exeComplete();
}

void NrvFlipPanelObserver::FlipPanelObserverNrvWait::execute(Spine *pSpine) const {
    FlipPanelObserver* obs = reinterpret_cast<FlipPanelObserver*>(pSpine->mExecutor);
    obs->exeWait();
}

void NrvFlipPanel::FlipPanelNrvEnd::execute(Spine *pSpine) const {
    FlipPanel* panel = reinterpret_cast<FlipPanel*>(pSpine->mExecutor);
    panel->exeEnd();
}

void NrvFlipPanel::FlipPanelNrvEndPrepare::execute(Spine *pSpine) const {
    FlipPanel* panel = reinterpret_cast<FlipPanel*>(pSpine->mExecutor);
    
    if (MR::isStep(panel, 0x14)) {
        panel->setNerve(&NrvFlipPanel::FlipPanelNrvEnd::sInstance);
    }
}

void NrvFlipPanel::FlipPanelNrvBack::execute(Spine *pSpine) const {
    FlipPanel* panel = reinterpret_cast<FlipPanel*>(pSpine->mExecutor);
    panel->exeWait();
}

void NrvFlipPanel::FlipPanelNrvFront::execute(Spine *pSpine) const {
    FlipPanel* panel = reinterpret_cast<FlipPanel*>(pSpine->mExecutor);
    panel->exeWait();
}

void NrvFlipPanel::FlipPanelNrvBackLand::execute(Spine *pSpine) const {
    FlipPanel* panel = reinterpret_cast<FlipPanel*>(pSpine->mExecutor);
    panel->exeBackLand();
}

void NrvFlipPanel::FlipPanelNrvFrontLand::execute(Spine *pSpine) const {
    FlipPanel* panel = reinterpret_cast<FlipPanel*>(pSpine->mExecutor);
    panel->exeFrontLand();
}

FlipPanel::~FlipPanel() {

}

FlipPanelObserver::~FlipPanelObserver() {
    
}