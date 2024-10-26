#include "Game/MapObj/GreenCaterpillarBig.hpp"

GreenCaterpillarBigBody::GreenCaterpillarBigBody(LiveActor *pCaterpillar, MtxPtr mtx) :
    ModelObj("オオムイムイ体", "GreenCaterpillarBigBody", mtx, -2, -2, -2, false) {
        mCaterpillar = pCaterpillar;
        mFrontVec.x = 0.0f;
        mFrontVec.y = 0.0f;
        mFrontVec.z = 1.0f;
        mPlanetLOD = nullptr;
    }

void GreenCaterpillarBigBody::init(const JMapInfoIter &rIter) {
    MR::invalidateClipping(this);
    mPlanetLOD = MR::createLodCtrlPlanet(this, rIter, -1.0f, -1);
    mPlanetLOD->validate();
    MR::calcFrontVec(&mFrontVec, mCaterpillar);
    MR::startBck(this, "Wait", nullptr);
    makeActorDead();
}

void GreenCaterpillarBigBody::setPosAndDirection(LiveActor *pActor) {
    TVec3f jointPos;
    MR::copyJointPos(mCaterpillar, "FollowPoint", &jointPos);
    mPosition.setInline(jointPos);
    calcBodyDir(pActor, &mFrontVec);

    mScale.x = 1.0f;
    mScale.y = 1.0f;
    mScale.z = 1.0f;
}

void GreenCaterpillarBigBody::control() {
    mPlanetLOD->update();
}

void GreenCaterpillarBigBody::calcAndSetBaseMtx() {
    if (MR::isNearZero(mFrontVec, 0.001f)) {
        LiveActor::calcAndSetBaseMtx();
    }
    else {
        MtxPtr jointMtx = MR::getJointMtx(mCaterpillar, "FollowPoint");
        TVec3f pos;
        pos.set(jointMtx[0][1], jointMtx[1][1], jointMtx[2][1]);
        TPos3f frontUpPos;
        MR::makeMtxFrontUpPos(&frontUpPos, mFrontVec, pos, mPosition);
        MR::setBaseTRMtx(this, frontUpPos);  
    }
}

void GreenCaterpillarBigBody::calcBodyDir(LiveActor *pActor, TVec3f *pOutDir) {
    f32 nearRailCoord = MR::calcNearestRailCoord(pActor, mPosition);
    f32 coord = nearRailCoord - 300.0f;
    if (coord <= 0.0f) {
        MR::calcRailDirectionAtCoord(pOutDir, pActor, nearRailCoord);
    }
    else {
        TVec3f railPos;  
        MR::calcRailPosAtCoord(&railPos, pActor, coord); 
        TVec3f stack_8;
        stack_8.subInline3(mPosition, railPos); 
        MR::normalize(&stack_8);
        MR::blendVec(pOutDir->toVec(), *pOutDir->toCVec(), *stack_8.toCVec(), 0.1f);
    }
}

GreenCaterpillarBig::GreenCaterpillarBig(const char *pName) : LiveActor(pName) {
    mBodyArray = nullptr;
    mBodyArrayLength = 0;
    mCurBodyParts = 0;
    _98 = 0;
    _9C = 0;
    _9D = 0;
    mPlanetLOD = nullptr;
}

void GreenCaterpillarBig::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("GreenCaterpillarBigFace", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    initEffectKeeper(0, nullptr, false);
    initSound(6, false);
    initRailRider(rIter);
    initBodyParts(rIter);
    MR::hideModel(this);
    MR::invalidateClipping(this);
    mPlanetLOD = MR::createLodCtrlPlanet(this, rIter, -1.0f, -1);
    mPlanetLOD->validate();
    initNerve(&NrvGreenCaterpillarBig::GreenCaterpillarBigNrvHide::sInstance);
    makeActorAppeared();
}

void GreenCaterpillarBig::startWriggle() {
    setNerve(&NrvGreenCaterpillarBig::GreenCaterpillarBigNrvWriggle::sInstance);
}

void GreenCaterpillarBig::exeWriggle() {
    if (MR::isFirstStep(this)) {
        if (MR::isHiddenModel(this)) {
            MR::showModel(this);
        }

        MR::startBck(this, "Eat", nullptr);
    }

    MR::moveCoordAndFollowTrans(this, 50.0f);
    MR::startLevelSound(this, "SE_OJ_LV_GRN_CATERP_MOVE", -1, -1, -1);

    if (MR::isRailReachedGoal(this)) {
        setNerve(&NrvGreenCaterpillarBig::GreenCaterpillarBigNrvEndAdjust::sInstance);
    }
    else {
        _9C = 0;
        _9D = 0;

        bool isNotEqual = MR::getCurrentRailPointNo(this) != _98;

        if (isNotEqual) {
            _98 = MR::getCurrentRailPointNo(this);
            f32 point_arg = -1.0f;
            MR::getRailPointArg0NoInit(this, _98, &point_arg);

            if (point_arg == 0.0f) {
                _9C = 1;
                MR::tryRumblePadStrong(this, 0);
                MR::shakeCameraNormal();
                MR::startSound(this, "SE_OJ_GRN_CATERP_IN", -1, -1);
            }
            else if (point_arg == 1.0f) {
                setNerve(&NrvGreenCaterpillarBig::GreenCaterpillarBigNrvRest::sInstance);
            }
            else if (point_arg == 2.0f) {
                leaveApple();
            }
        }
    }
}

void GreenCaterpillarBig::initBodyParts(const JMapInfoIter &rIter) {
    s32 count = MR::getRailTotalLength(this) / 300.0f;
    mBodyArrayLength = count;
    mBodyArray = new GreenCaterpillarBigBody*[count];
    GreenCaterpillarBig* caterpillar = this;

    for (s32 i = 0; i < mBodyArrayLength; i++) {
        MtxPtr jointMtx = MR::getJointMtx(caterpillar, "FollowPoint");
        mBodyArray[i] = new GreenCaterpillarBigBody(caterpillar, jointMtx);
        mBodyArray[i]->init(rIter);
        caterpillar = reinterpret_cast<GreenCaterpillarBig*>(mBodyArray[i]);
    }
}

bool GreenCaterpillarBig::tryGenerateBodyParts() {
    if (mCurBodyParts >= mBodyArrayLength) {
        return false;
    }

    f32 coord = !mCurBodyParts ? MR::getRailCoord(this) : MR::calcNearestRailCoord(this, mBodyArray[mCurBodyParts - 1]->mPosition);

    if (coord < 300.0f) {
        return false;
    }

    mBodyArray[mCurBodyParts]->appear();
    mCurBodyParts++;
    return true;
}

void GreenCaterpillarBig::control() {
    if (!isNerve(&NrvGreenCaterpillarBig::GreenCaterpillarBigNrvHide::sInstance)) {
        if (!isNerve(&NrvGreenCaterpillarBig::GreenCaterpillarBigNrvEnd::sInstance)) {
            tryGenerateBodyParts();
        }

        fixBodyPartsOnRail();
        mPlanetLOD->update();
    }
}

void GreenCaterpillarBig::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, MR::getRailDirection(this));
    MR::setBaseTRMtx(this, mtx);
}

void GreenCaterpillarBig::startClipped() {
    for (s32 i = 0; i < mCurBodyParts; i++) {
        MR::validateClipping(mBodyArray[i]);
    }

    LiveActor::startClipped();
}

void GreenCaterpillarBig::endClipped() {
    for (s32 i = 0; i < mCurBodyParts; i++) {
        MR::invalidateClipping(mBodyArray[i]);
    }

    LiveActor::endClipped();
}

void GreenCaterpillarBig::fixBodyPartsOnRail() {
    for (s32 i = 0; i < mCurBodyParts; i++) {
        mBodyArray[i]->setPosAndDirection(this);
    }
}

void GreenCaterpillarBig::leaveApple() {
    _9D = 1;
    MR::tryRumblePadStrong(this, 0);
    MR::shakeCameraNormal();
    MR::startSound(this, "SE_OJ_GRN_CATERP_DAMAGE", -1, -1);
    MR::startSound(this, "SE_OJ_GRN_CATERP_OUT", -1, -1);
}

GreenCaterpillarBigBody::~GreenCaterpillarBigBody() {

}

GreenCaterpillarBig::~GreenCaterpillarBig() {

}

namespace NrvGreenCaterpillarBig {
    INIT_NERVE(GreenCaterpillarBigNrvHide);
    INIT_NERVE(GreenCaterpillarBigNrvWriggle);
    INIT_NERVE(GreenCaterpillarBigNrvRest);
    INIT_NERVE(GreenCaterpillarBigNrvEndAdjust);
    INIT_NERVE(GreenCaterpillarBigNrvEnd);

    void GreenCaterpillarBigNrvEnd::execute(Spine *pSpine) const {
        GreenCaterpillarBig* caterpillar = reinterpret_cast<GreenCaterpillarBig*>(pSpine->mExecutor);
        if (MR::isFirstStep(caterpillar)) {
            MR::startBck(caterpillar, "Wait", nullptr);
        }
    }

    void GreenCaterpillarBigNrvEndAdjust::execute(Spine *pSpine) const {
        GreenCaterpillarBig* caterpillar = reinterpret_cast<GreenCaterpillarBig*>(pSpine->mExecutor);
        if (MR::isStep(caterpillar, 0x78)) {
            caterpillar->setNerve(&NrvGreenCaterpillarBig::GreenCaterpillarBigNrvEnd::sInstance);
        }
    }

    void GreenCaterpillarBigNrvRest::execute(Spine *pSpine) const {
        GreenCaterpillarBig* caterpillar = reinterpret_cast<GreenCaterpillarBig*>(pSpine->mExecutor);
        MR::startLevelSound(caterpillar, "SE_OJ_LV_GRN_CATERP_EAT", -1, -1, -1);
    }

    void GreenCaterpillarBigNrvWriggle::execute(Spine *pSpine) const {
        GreenCaterpillarBig* caterpillar = reinterpret_cast<GreenCaterpillarBig*>(pSpine->mExecutor);
        caterpillar->exeWriggle();
    }

    void GreenCaterpillarBigNrvHide::execute(Spine *pSpine) const {
        GreenCaterpillarBig* caterpillar = reinterpret_cast<GreenCaterpillarBig*>(pSpine->mExecutor);
        MR::startLevelSound(caterpillar, "SE_OJ_LV_GRN_CATERP_EAT", -1, -1, -1);
    }
};
