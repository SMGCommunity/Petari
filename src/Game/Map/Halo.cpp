#include "Game/Map/Halo.hpp"

Halo::Halo(const char *pName) : MapObjActor(pName) {
    mDistance = 70.0f;
}

/*
void Halo::init(const JMapInfoIter &rIter) { 
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("環境");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupNerve(&NrvHalo::HostTypeAppear::sInstance);
    info.setupClippingRadius(100.0f * getParam()->clippingRadius);
    info.setupFarClipping(-1.0f);
    info.setupAffectedScale();
    initialize(rIter, info);
    MR::getJMapInfoArg0NoInit(rIter, &mDistance);
}
*/

void Halo::appear() {
    MapObjActor::appear();

    if (isDistanceDisappear()) {
        const char* anim = "Disappear";
        MR::tryStartAllAnim(this, anim);
        if (MR::isExistBtk(this, anim)) {
            J3DFrameCtrl* ctrl = MR::getBtkCtrl(this);
            MR::setBtkFrame(this, ctrl->mEndFrame);
        }

        if (MR::isExistBpk(this, anim)) {
            J3DFrameCtrl* ctrl = MR::getBpkCtrl(this);
            MR::setBpkFrame(this, ctrl->mEndFrame);
        
        }
        if (MR::isExistBtp(this, anim)) {
            J3DFrameCtrl* ctrl = MR::getBtpCtrl(this);
            MR::setBpkFrame(this, ctrl->mEndFrame);
        }
        
        if (MR::isExistBrk(this, anim)) {
            J3DFrameCtrl* ctrl = MR::getBrkCtrl(this);
            MR::setBrkFrame(this, ctrl->mEndFrame);
        }

        MR::hideModel(this);
        setNerve(&NrvHalo::HostTypeDisappear::sInstance);
    }
    else {
        MR::startAllAnim(this, "Appear");
        setNerve(&NrvHalo::HostTypeAppear::sInstance);
    }
}

f32 Halo::getDistance() const {
    return MR::calcDistanceToPlayer(this);
}

void Halo::connectToScene(const MapObjActorInitInfo &rIter) {
    MR::connectToSceneAir(this);
}

bool Halo::isDistanceAppear() const {
    f32 v1 = (mDistance + 20.0f) * 100.0f;

    f32 d = mDistance * 100.0f;
    if (d < v1) {
        return v1 < getDistance();
    }
    else {
        return getDistance() < v1;   
    }
}

/*
bool Halo::isDistanceDisappear() const {
    f32 v1 = (mDistance + 20.0f) * 100.0f;

    f32 d = mDistance * 100.0f;
    if (v1 < d) {
        return getDistance() < v1;
    }
    else {
        return v1 < getDistance();
    }
}
*/

void Halo::exeDisappear() {
    if (!MR::isHiddenModel(this)) {
        bool flag;
        const char* anim = "Disappear";

        if (MR::isExistBtk(this, anim) && MR::isBtkStopped(this)) {
            flag = true;
        }
        else if (MR::isExistBpk(this, anim) && MR::isBpkStopped(this)) {
            flag = true;
        }
        else if (MR::isExistBtp(this, anim) && MR::isBtpStopped(this)) {
            flag = true;
        }
        else if (MR::isExistBrk(this, anim) && MR::isBrkStopped(this)) {
            flag = true;
        }
        else {
            flag = false;
        }

        if (flag) {
            MR::hideModel(this);
        }
    }

    if (isDistanceAppear()) {
        MR::showModel(this);
        MR::startAllAnim(this, "Appear");
        setNerve(&NrvHalo::HostTypeAppear::sInstance);
    }
}

PowerStarHalo::PowerStarHalo(const char *pName) : Halo(pName) {
    _C8 = -1;
}

Halo::~Halo() {

}

void PowerStarHalo::init(const JMapInfoIter &rIter) {
    Halo::init(rIter);
    MR::getJMapInfoArg1NoInit(rIter, &_C8);
    if (_C8 == -1) {
        setNerve(&NrvPowerStarHalo::HostTypeWaitScenarioOpeningEnd::sInstance);
    }
}

void PowerStarHalo::appear() {
    if (isNerve(&NrvPowerStarHalo::HostTypeWaitScenarioOpeningEnd::sInstance)) {
        makeActorAppeared();
    }
    else {
        Halo::appear();
    }
}

f32 PowerStarHalo::getDistance() const {
    TVec3f pos = MR::getCamPos();
    return PSVECDistance(&mPosition, &pos);
}

void PowerStarHalo::exeWaitScenarioOpeningEnd() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
    }

    if (!MR::isStageStateScenarioOpeningCamera()) {
        MR::showModel(this);
        Halo::appear();
    }
}

PowerStarHalo::~PowerStarHalo() {

}

/* todo -- ordering of these functions is wrong */ 
void NrvPowerStarHalo::HostTypeWaitScenarioOpeningEnd::execute(Spine *pSpine) const {
    PowerStarHalo* halo = reinterpret_cast<PowerStarHalo*>(pSpine->mExecutor);
    halo->exeWaitScenarioOpeningEnd();
}

void NrvHalo::HostTypeDisappear::execute(Spine *pSpine) const {
    Halo* halo = reinterpret_cast<Halo*>(pSpine->mExecutor);
    halo->exeDisappear();
}

void NrvHalo::HostTypeAppear::execute(Spine *pSpine) const {
    Halo* halo = reinterpret_cast<Halo*>(pSpine->mExecutor);
    if (halo->isDistanceDisappear()) {
        MR::startAllAnim(halo, "Disappear");
        halo->setNerve(&NrvHalo::HostTypeDisappear::sInstance);
    }
}
