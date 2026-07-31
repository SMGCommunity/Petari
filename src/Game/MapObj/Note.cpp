#include "Game/MapObj/Note.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/NoteFairy.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/NoteCounter.hpp"
#include "Game/Util.hpp"

void Note_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)-1.0f;
}

namespace {
    static const f32 sRotateSpeed = 8.0f;
    static const s32 sStepFlyUp = 10;
    static const f32 sFlyUpSpeed = 30.0f;
};  // namespace

namespace NrvNote {
    NEW_NERVE(NoteNrvWait, Note, Wait);
    NEW_NERVE(NoteNrvCountDown, Note, CountDown);
    NEW_NERVE(NoteNrvFlyUp, Note, FlyUp);
};  // namespace NrvNote

Note::Note(const char* pName, const TVec3f& rRailDirection, NoteFairy* pParent) : LiveActor(pName) {
    mParentFairy = pParent;
    _90.set(0.0f);
    mRailDirection = rRailDirection;
    mCounter = 0;
    _AC = 0.0f;
    mIsCountdown = false;
    mFlashCtrl = nullptr;
}

void Note::init(const JMapInfoIter& rIter) {
    MR::createSceneObj(SceneObj_NoteGroup);
    _90.set(mPosition);
    initModelManagerWithAnm("Note", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 8, 80.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initNerve(&NrvNote::NoteNrvWait::sInstance);
    mFlashCtrl = new FlashingCtrl(this, true);
    MR::calcGravityAndDropShadowVector(this, &mGravity, nullptr, 0);

    if (MR::isNearZero(mGravity)) {
        mGravity.set(0.0f, -1.0f, 0.0);
    }

    MR::initShadowVolumeSphere(this, 30.0f);
    MR::turnVecToPlane(&mRailDirection, mRailDirection, mGravity);
    MR::invalidateClipping(this);
    makeActorDead();
}

void Note::startCountDown() {
    mIsCountdown = true;
}

void Note::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Note");
        f32 framemax = MR::getBrkFrameMax(this, "Note");
        MR::setBrkFrame(this, MR::repeat(_AC, 0.0f, framemax));
    }

    if (mIsCountdown) {
        setNerve(&NrvNote::NoteNrvCountDown::sInstance);
    }
}

void Note::exeCountDown() {
    if (MR::isFirstStep(this)) {
        mFlashCtrl->start(mCounter);
    }

    if (MR::isStep(this, mCounter)) {
        MR::emitEffect(this, "Vanish");
        MR::startSound(this, "SE_OJ_MUSICAL_NOTE_BREAK");
        MR::failureNoteCollection();
        kill();
    }
}

void Note::exeFlyUp() {
    if (MR::isFirstStep(this)) {
        mVelocity.set(mGravity);
        mVelocity *= -::sFlyUpSpeed;
        MR::invalidateHitSensors(this);
        mFlashCtrl->end();
    }

    if (MR::isStep(this, ::sStepFlyUp)) {
        kill();
    }
}

void Note::control() {
    mRotation.y = MR::getSceneObj< NoteGroup >(SceneObj_NoteGroup)->mRotation;
}

void Note::calcAndSetBaseMtx() {
    TPos3f stack_44;
    MR::makeMtxUpFront(&stack_44, -mGravity, mRailDirection);
    Mtx mtxRotateY;
    f32 val = MR::getSceneObj< NoteGroup >(SceneObj_NoteGroup)->mRotation;
    MR::makeMtxRotateY(mtxRotateY, val);
    MR::multMtx(stack_44, mtxRotateY, stack_44);
    stack_44.setTrans(mPosition);
    MR::setBaseTRMtx(this, stack_44);
}

void Note::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayerOrRide(pReceiver)) {
        if (!isNerve(&NrvNote::NoteNrvFlyUp::sInstance)) {
            MR::emitEffect(this, "Get");
            MR::addNoteToCounter();
            mParentFairy->setLastGotNote(this);
            setNerve(&NrvNote::NoteNrvFlyUp::sInstance);
        }
    }
}

NoteGroup::NoteGroup() : LiveActorGroup("音符グループ", 0x100) {
    mRotation = 0.0f;
}

void NoteGroup::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
}

void NoteGroup::movement() {
    mRotation += ::sRotateSpeed;
    MR::repeatDegree(&mRotation);
}
