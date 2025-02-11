#include "Game/MapObj/Note.hpp"
#include "Game/MapObj/NoteFairy.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/NoteCounter.hpp"

Note::Note(const char *pName, const TVec3f &rRailDirection, NoteFairy *pParent) : LiveActor(pName) {
    mParentFairy = pParent;
    _90.set(0.0f);
    mRailDirection.setPS(rRailDirection);
    mCounter = 0;
    _AC = 0.0f;
    mIsCountdown = false;
    mFlashCtrl = nullptr;
}

void Note::init(const JMapInfoIter &rIter) {
    MR::createSceneObj(SceneObj_NoteGroup);
    _90.set<f32>(mPosition);
    initModelManagerWithAnm("Note", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);
    initHitSensor(1);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 0.0f;
    offs.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 8, 80.0f, offs);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initNerve(&NrvNote::NoteNrvWait::sInstance);
    mFlashCtrl = new FlashingCtrl(this, true);
    MR::calcGravityAndDropShadowVector(this, &mGravity, nullptr, 0);
    if (MR::isNearZero(mGravity, 0.001f)) {
        mGravity.x = 0.0f;
        mGravity.y = -1.0f;
        mGravity.z = 0.0f;
    }

    MR::initShadowVolumeSphere(this, 30.0f);
    MR::turnVecToPlane(&mRailDirection, mRailDirection, mGravity);
    MR::invalidateClipping(this);
    makeActorDead();
}

void Note::startCountDown() {
    mIsCountdown = true;
}

/*
void Note::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Note");
        f32 framemax = MR::getBrkFrameMax(this, "Note");
        f32 rpt = MR::repeat(_AC, 0.0f, framemax);
        MR::setBrkFrame(this, rpt);
    }

    if (mIsCountdown) {
        setNerve(&NrvNote::NoteNrvCountDown::sInstance);
    } 
}
*/

void Note::exeCountDown() {
    if (MR::isFirstStep(this)) {
        mFlashCtrl->start(mCounter);
    }

    if (MR::isStep(this, mCounter)) {
        MR::emitEffect(this, "Vanish");
        MR::startSound(this, "SE_OJ_MUSICAL_NOTE_BREAK", -1, -1);
        MR::failureNoteCollection();
        kill();
    }
}

/*
void Note::exeFlyUp() {
    if (MR::isFirstStep(this)) {
        mVelocity.set<f32>(mGravity);
        mVelocity.scaleInline(-30.0f);
        MR::invalidateHitSensors(this);
        mFlashCtrl->end();
    }

    if (MR::isStep(this, 10)) {
        kill();
    }
}
*/

void Note::control() {
    mRotation.y = MR::getSceneObj<NoteGroup*>(SceneObj_NoteGroup)->mRotation;
}

/*
void Note::calcAndSetBaseMtx() {
    TVec3f stack_8;
    stack_8.negateInline_2(mGravity);
    TPos3f stack_44;
    MR::makeMtxUpFront(&stack_44, stack_8, mRailDirection);
    Mtx mtxRotateY;
    f32 val = MR::getSceneObj<NoteGroup*>(SceneObj_NoteGroup)->mRotation;
    MR::makeMtxRotateY(mtxRotateY, val);
    PSMTXConcat(stack_44.toMtxPtr(), mtxRotateY, stack_44.toMtxPtr());
    stack_44.mMtx[0][3] = mPosition.x;
    stack_44.mMtx[1][3] = mPosition.y;
    stack_44.mMtx[2][3] = mPosition.z;
    MR::setBaseTRMtx(this, stack_44);
}
*/

void Note::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorPlayerOrRide(a2)) {
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

void NoteGroup::init(const JMapInfoIter &rIter) {
    MR::connectToSceneMapObjMovement(this);
}

/*
void NoteGroup::movement() {
    f32 v1 = MR::subtractFromSum(8.0f, mRotation, 0.0f);
    mRotation = mRotation + 8.0f;
    mRotation = MR::modAndAdd(0.0f, v1);
}
*/

Note::~Note() {

}

NoteGroup::~NoteGroup() {

}

namespace NrvNote {
    INIT_NERVE(NoteNrvWait);
    INIT_NERVE(NoteNrvCountDown);
    INIT_NERVE(NoteNrvFlyUp);
};
