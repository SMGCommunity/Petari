#include "Game/Map/SeaGull.hpp"
#include "Game/Util.hpp"

NrvSeaGull::SeaGullNrvHoverFront NrvSeaGull::SeaGullNrvHoverFront::sInstance;
NrvSeaGull::SeaGullNrvHoverLeft NrvSeaGull::SeaGullNrvHoverLeft::sInstance;
NrvSeaGull::SeaGullNrvHoverRight NrvSeaGull::SeaGullNrvHoverRight::sInstance;

SeaGull::SeaGull(SeaGullGroup *pGroup) : LiveActor("カモメ") {
    mSeaGullGroup = pGroup;
    _90 = 0;
    _94 = false;
    _98 = MR::getRandom((s32)0, (s32)0xB4);
    _9C = 0;
    _A0.x = 0.0f;
    _A0.y = 1.0f;
    _A0.z = 0.0f;
    _AC.x = 0.0f;
    _AC.y = 0.0f;
    _AC.z = 1.0f;
    _B8.x = 0.0f;
    _B8.y = 1.0f;
    _B8.z = 0.0f;
    _C4.x = 1.0f;
    _C4.y = 0.0f;
    _C4.z = 0.0f;
    _D0 = 0;
    _D4 = 0.0f;
    _D8 = 0;
    _DC = 0;
    _E0 = 0;
}

#ifdef NON_MATCHING
void SeaGull::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::calcActorAxis(&_C4, &_B8, &_AC, this);
    MR::calcActorAxisY(&_A0, this);
    initModelManagerWithAnm("SeaGull", 0, false);
    MR::startBck(this, "Fly", 0);
    MR::setBckFrameAtRandom(this);
    MR::connectToSceneEnvironment(this);
    f32 railLength = MR::getRailTotalLength(mSeaGullGroup);
    f32 diff = railLength - 1.0f;
    f32 rnd = MR::getRandom(1.0f, diff);
    f64 val = rnd / 500.0f;
    _90 = val;
    MR::calcRailPosAtCoord(&mPosition, mSeaGullGroup, rnd);
    u8 a = MR::isHalfProbability();
    _94 = a;
    _9C = mSeaGullGroup->updatePosInfo(&_90, a);
 
    TVec3f stack_14(_C4);
    TVec3f stack_8(_AC); 
    f32 scale = MR::getRandom(-1.0f, 1.0f);
    stack_14.scale(scale);
    scale = MR::getRandom(-1.0f, 1.0f);
    stack_8.scale(scale); 
    _AC.set(stack_14);
    _AC.add(stack_8);

    if (MR::isNearZero(_AC, 0.001f)) {
        _AC.x = 0.0f;
        _AC.y = 0.0f;
        _AC.z = 1.0f;
    }
    else {
        MR::normalize(&_AC);
    }

    initNerve(&NrvSeaGull::SeaGullNrvHoverFront::sInstance);
    initSound(4, false);
    _E0 = MR::getRandom((s32)0x3C, (s32)0x1E0);
    MR::invalidateClipping(this);
    makeActorAppeared();
}
#endif

void SeaGull::exeHoverFront() {
    if (MR::isFirstStep(this)) {
        _D0 = MR::getRandom((s32)0, (s32)0x3C);
    }

    _D4 *= 0.995f;

    if (_D0 >= 0) {
        _D0 = _D0 - 1;
    }
    else {
        TVec3f stack_8(*_9C);
        stack_8.sub(mPosition);
        f32 dist = PSVECDistance((const Vec*)&mPosition, (const Vec*)_9C);

        if (dist > 500.0f) {
            f32 prod = _C4.dot(stack_8);

            if (prod > 0.0f) {
                setNerve(&NrvSeaGull::SeaGullNrvHoverLeft::sInstance);
            }
            else {
                setNerve(&NrvSeaGull::SeaGullNrvHoverRight::sInstance);
            }
        }
    }
}

void SeaGull::exeHoverLeft() {
    if (MR::isFirstStep(this)) {
        _D0 = MR::getRandom((s32)0x3C, (s32)0x78);
    }

    _D4 -= 0.1f;

    if (MR::isStep(this, _D0)) {
        setNerve(&NrvSeaGull::SeaGullNrvHoverFront::sInstance);
    }
}

void SeaGull::exeHoverRight() {
    if (MR::isFirstStep(this)) {
        _D0 = MR::getRandom((s32)0x3C, (s32)0x78);
    }

    _D4 += 0.1f;

    if (MR::isStep(this, _D0)) {
        setNerve(&NrvSeaGull::SeaGullNrvHoverFront::sInstance);
    }
}

void SeaGull::control() {
    updateHover();

    mVelocity.x *= 0.99000001f;
    mVelocity.y *= 0.99000001f;
    mVelocity.z *= 0.99000001f;

    f32 mag = PSVECMag((const Vec*)&mVelocity);

    if (mag > 0.0f) {
        MR::normalize(&mVelocity);
        mVelocity.x *= 10.0f;
        mVelocity.y *= 10.0f;
        mVelocity.z *= 10.0f;
    }

    PSVECCrossProduct((const Vec*)&_B8, (const Vec*)&_AC, (Vec*)&_C4);
    MR::normalize(&_C4);
    PSVECCrossProduct((const Vec*)&_AC, (const Vec*)&_C4, (Vec*)&_B8);
    MR::normalize(&_B8);
    _98 -= 1;

    if (_98 <= 0) {
        _9C = mSeaGullGroup->updatePosInfo(&_90, _94);
        _98 = 0xB4; 
    } 

    if (_E0 <= 0) {
        MR::startSound(this, "SE_OJ_SEAGULL_CHIRP", -1, -1);
        _E0 = MR::getRandom((s32)0x3C, (s32)0x1E0);
    }
    else {
        _E0--;
    }

    if (MR::isJudgedToClipFrustum300m(mPosition, 200.0f)) {
        if (!MR::isHiddenModel(this)) {
            MR::hideModel(this);
        }
    }
    else {
        if (MR::isHiddenModel(this)) {
            MR::showModel(this);
        }
    }
}

void SeaGull::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx.identity();
    mtx.mMtx[0][0] = _C4.x;
    mtx.mMtx[1][0] = _C4.y;
    mtx.mMtx[2][0] = _C4.z;
    mtx.mMtx[0][1] = _B8.x;
    mtx.mMtx[1][1] = _B8.y;
    mtx.mMtx[2][1] = _B8.z;
    mtx.mMtx[0][2] = _AC.x;
    mtx.mMtx[1][2] = _AC.y;
    mtx.mMtx[2][2] = _AC.z;
    mtx.mMtx[0][3] = mPosition.x;
    mtx.mMtx[1][3] = mPosition.y;
    mtx.mMtx[2][3] = mPosition.z;
    MR::setBaseTRMtx(this, mtx);
}

SeaGullGroup::SeaGullGroup(const char *pName) : LiveActor(pName) {
    _8C = 0;
    _90 = 0;
}

TVec3f* SeaGullGroup::updatePosInfo(s32 *a1, bool a2) const {
    if (a2) {
        if (--*a1 <= 0) {
            *a1 = _8C - 1;
        }
    }
    else {
        s32 val = _8C;
        s32 next = *a1 + 1;
        *a1 = next;

        if (next >= val) {
            *a1 = 0;
        }
    }

    return &_90[*a1];
}

SeaGull::~SeaGull() {

}

SeaGullGroup::~SeaGullGroup() {

}

namespace NrvSeaGull {
    void SeaGullNrvHoverRight::execute(Spine *pSpine) const {
        SeaGull* gull = reinterpret_cast<SeaGull*>(pSpine->mExecutor);
        gull->exeHoverRight();
    }

    void SeaGullNrvHoverLeft::execute(Spine *pSpine) const {
        SeaGull* gull = reinterpret_cast<SeaGull*>(pSpine->mExecutor);
        gull->exeHoverLeft();
    } 

    void SeaGullNrvHoverFront::execute(Spine *pSpine) const {
        SeaGull* gull = reinterpret_cast<SeaGull*>(pSpine->mExecutor);
        gull->exeHoverFront();
    } 
};
