#include "Game/MapObj/NoteFairy.hpp"
#include "Game/MapObj/Note.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/NoteCounter.hpp"

NoteFairy::NoteFairy(const char *pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = 0.0f;
    _8C.z = 0.0f;
    _98.x = 0.0f;
    _98.y = 0.0f;
    _98.z = 0.0f;
    mAppearanceSpeed = 13.0f;
    mTimeLimit = 20;
    mMelodyNoteNum = 0;
    _B0 = 0;
    mSong = -1;
    mCompletionJingle = -1;
    mNoteCoord = 150.0f;
    mNoteArray = nullptr;
    mLastGotNote = nullptr;
    mAppearanceType = -1;
    _CC = false;
    _CD = false;
    mHasDemo = false;
    _CF = false;
    _D0 = false;
    _D1 = false;
    mPowerStarSpawnType = -1;
    mCameraInfo = nullptr;
}

void NoteFairy::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    s32 noteNum = -1;
    MR::getJMapInfoArg0WithInit(rIter, &noteNum);
    MR::getJMapInfoArg1NoInit(rIter, &mAppearanceSpeed);
    MR::getJMapInfoArg2NoInit(rIter, &mTimeLimit);
    mAppearanceType = -1;
    MR::getJMapInfoArg3WithInit(rIter, &mAppearanceType);

    switch (mAppearanceType) {
        case -1: break;
        case 1: break;
        case 3: _CC = 1; break;
        case 5: _CD = 1; break;
        case 6: _CD = 1; _D0 = 1; break;
    }

    MR::getJMapInfoArg4WithInit(rIter, &mSong);
    MR::getJMapInfoArg5WithInit(rIter, &mCompletionJingle);
    s32 arg6;
    MR::getJMapInfoArg6WithInit(rIter, &arg6);

    if (arg6 > 0) {
        _CF = false;   
    }
    else {
        _CF = true;
    }

    s32 powerStarType = 0;
    MR::getJMapInfoArg7NoInit(rIter, &powerStarType);

    if (powerStarType != 0) {
        MR::declarePowerStar(this);

        if (powerStarType == 2) {
            mPowerStarSpawnType = 1;
        }
        else if (powerStarType == 3) {
            mPowerStarSpawnType = 2;
        }
        else {
            mPowerStarSpawnType = 0;
        }
    }

    _8C.set<f32>(mPosition);
    mTimeLimit *= 60;
    mHasDemo = MR::tryRegisterDemoCast(this, rIter);
    initRailRider(rIter);
    initModelManagerWithAnm("Note", nullptr, false);
    MR::startBrk(this, "Note");
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);
    initEffectKeeper(0, "NoteFairy", false);
    initSound(6, false);
    MR::initShadowVolumeSphere(this, 50.0f);
    s32 railArg = -1;
    MR::getRailArg0WithInit(this, &railArg);

    if (noteNum > 0) {
        if (mSong < 0) {
            mMelodyNoteNum = noteNum;
        }
        else {
            mMelodyNoteNum = MR::getRemixMelodyNoteNum(mSong);
        }

        mNoteCoord = MR::getRailTotalLength(this) / mMelodyNoteNum;
    }
    else {
        if (noteNum == -2) {

            s32 pointNum = MR::getRailPointNum(this);
            if (mSong < 0) {
                mMelodyNoteNum = pointNum;
            }
            else {
                mMelodyNoteNum = MR::getRemixMelodyNoteNum(mSong);
            }

            if (railArg < 0) {
                mNoteCoord = 200.0f;
            }
            else {
                mNoteCoord = railArg;
            }

            _D1 = true;
        }
        else {
            if (mSong < 0) { 
                mMelodyNoteNum = MR::getRailTotalLength(this) / mNoteCoord;
            }
            else {
                mMelodyNoteNum = MR::getRemixMelodyNoteNum(mSong);
                mNoteCoord = MR::getRailTotalLength(this) / mMelodyNoteNum;
            }
        }
    }

    mNoteArray = new Note*[mMelodyNoteNum];

    for (s32 i = 0; i < mMelodyNoteNum; i++) {
        TVec3f stack_24;

        if (_D1) {
            MR::calcRailPointPos(&stack_24, this, i);
        }
        else {
            stack_24.set<f32>(MR::getRailPos(this));
            MR::moveCoord(this, mNoteCoord);
        }

        mNoteArray[i] = new Note("音符", MR::getRailDirection(this), this);
        mNoteArray[i]->mPosition.set<f32>(stack_24);
        mNoteArray[i]->initWithoutIter();
        mNoteArray[i]->mCounter = mTimeLimit;
    }

    MR::moveCoordToStartPos(this);
    MR::setRailCoord(this, 0.0f);
    TVec3f stack_18(MR::getRailDirection(this));
    stack_18.y = 0.0f;
    MR::normalizeOrZero(&stack_18);
    stack_18.x *= -800.0f;
    stack_18.y *= -800.0f;
    stack_18.z *= -800.0f;
    _98.x = stack_18.x + mPosition.x;
    _98.y = (0.5f * mPosition.y) + (0.5f * MR::getRailPos(this).y);
    _98.z = stack_18.z + mPosition.z;
    MR::needStageSwitchReadAppear(this, rIter);
    MR::useStageSwitchWriteA(this, rIter);
    MR::useStageSwitchReadB(this, rIter);
    MR::initActorCamera(this, rIter, &mCameraInfo);
    initNerve(&NrvNoteFairy::NoteFairyNrvHide::sInstance);
    makeActorAppeared();
}

void NoteFairy::control() {
    if (!isNerve(&NrvNoteFairy::NoteFairyNrvHide::sInstance) && MR::isValidSwitchB(this) && !MR::isOnSwitchB(this)) {
        kill();
    }
    else if (MR::isPowerStarGetDemoActive()) {
        kill();
    }
}

void NoteFairy::kill() {
    if (_CF) {
        MR::moveVolumeStageBGMForNoteFairy(1.0f, 120);
    }

    killAllNotes();
    LiveActor::kill();
}

void NoteFairy::killAllNotes() {
    for (s32 i = 0; i < mMelodyNoteNum; i++) {
        mNoteArray[i]->kill();
    }
}

void NoteFairy::exeHide() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::invalidateClipping(this);
    }

    if (MR::isOnSwitchAppear(this)) {
        if (!_D0) {
            MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
        }

        MR::invalidateClipping(this);

        if (_CF) {
            MR::moveVolumeStageBGMForNoteFairy(0.0f, 30);
        }

        switch(mAppearanceType) {
            case 1:
            case 2:
            case 3:
                setNerve(&NrvNoteFairy::NoteFairyNrvStartAppearDemo::sInstance);
                return;
            case 4:
            case 5:
            case 6:
                setNerve(&NrvNoteFairy::NoteFairyNrvRailMoveStart::sInstance);
                return;
        }

        setNerve(&NrvNoteFairy::NoteFairyNrvAppearNote::sInstance);
    }
}

/*
void NoteFairy::exeStartAppearDemo() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::emitEffect(this, "Blur");
    }

    if (MR::isStep(this, 30)) {
        TVec3f* pos = MR::getPlayerPos();
        TVec3f stack_8(_8C);
        stack_8.subInline(*pos); 
        MR::setPlayerFrontVec(stack_8, 1);
        enterDemoAppear(&NrvNoteFairy::NoteFairyNrvDemoAppear::sInstance, true);
    }
}
*/

void NoteFairy::exeRailMoveStart() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::emitEffect(this, "Blur");
        mPosition.setPS(MR::getRailPos(this));
    }

    if (_CD) {
        setNerve(&NrvNoteFairy::NoteFairyNrvAppearNoteBloom::sInstance);
    }
    else if (MR::isStep(this, 0)) {
        enterDemoAppear(&NrvNoteFairy::NoteFairyNrvAppearNoteBloom::sInstance, false);
    }
}

void NoteFairy::exeDemoAppear() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Blur");
    }

    if (MR::isLessEqualStep(this, 60)) {
        f32 ease = MR::getEaseOutValue(getNerveStep() / 60.0f, 0.0f, 1.0f, 1.0f);
        mPosition.x = (_8C.x * (1.0f - ease)) + (_98.x * ease);
        mPosition.y = (((_8C.y * (1.0f - ease)) + (_98.y * ease)) - (300.0f * MR::sinDegree(180.0f * ease)));
        mPosition.z = (_8C.z * (1.0f - ease)) + (_98.z * ease);
    }

    if (MR::isGreaterStep(this, 70)) {
        f32 ease = MR::getEaseInValue(getNerveStep() / 60.0f, 0.0f, 1.0f, 1.0f);
        mPosition.x = (_98.x * (1.0f - ease)) + (ease * MR::getRailPos(this).x);
        mPosition.y = _98.y - (MR::sinDegree(90.0f * ease) * (_98.y - MR::getRailPos(this).y));
        mPosition.z = (_98.z * (1.0f - ease)) + (ease * MR::getRailPos(this).z);
    }

    MR::startLevelSound(this, "SE_OJ_LV_FLOWER_FAIRY_MOVE", -1, -1, -1);
    mRotation.y = MR::getSceneObj<NoteGroup*>(SceneObj_NoteGroup)->mRotation;

    if (MR::isStep(this, 130)) {
        setNerve(&NrvNoteFairy::NoteFairyNrvAppearNoteBloom::sInstance);
    }
}

void NoteFairy::exeAppearNoteBloom() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
        MR::declareNoteNumMaxAndMelodyToCounter(this, mMelodyNoteNum, mSong, mNoteCoord);
    }

    if (!_CC && !_CD && MR::isStep(this, 60) && mAppearanceType != -1) {
        MR::endDemo(this, "出現");
        MR::endActorCamera(this, mCameraInfo, false, -1);
    }

    MR::startLevelSound(this, "SE_OJ_LV_FLOWER_FAIRY_MOVE", -1, -1, -1);
    MR::moveCoordAndFollowTrans(this, mAppearanceSpeed);
    mRotation.y = MR::getSceneObj<NoteGroup*>(SceneObj_NoteGroup)->mRotation;

    while (_B0 >= mMelodyNoteNum && MR::getRailCoord(this) >= getNoteCoord(_B0)) {
        mNoteArray[_B0]->appear();
        mNoteArray[_B0]->_AC = 5.0f * _B0;

        if (MR::isLessStep(this, 60) && mAppearanceType != -1) {
            MR::requestMovementOn(mNoteArray[_B0]);
        }

        _B0++;
    }

    if (_B0 >= mMelodyNoteNum) {
        if (_CC) {
            MR::endDemo(this, "出現");
            MR::endActorCamera(this, mCameraInfo, false, -1);
        }

        MR::hideModel(this);
        MR::deleteEffect(this, "Blur");
        setNerve(&NrvNoteFairy::NoteFairyNrvEndWait::sInstance);
    }
}

void NoteFairy::exeAppearNote() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
        MR::declareNoteNumMaxAndMelodyToCounter(this, mMelodyNoteNum, mSong, mNoteCoord);

        for (s32 i = 0; i < mMelodyNoteNum; i++) {
            mNoteArray[i]->appear();
            mNoteArray[i]->_AC = 5.0f * i;
        }

        setNerve(&NrvNoteFairy::NoteFairyNrvEndWait::sInstance);
    }
}

void NoteFairy::exeEndWait() {
    if (MR::isFirstStep(this)) {
        for (s32 i = 0; i < mMelodyNoteNum; i++) {
            mNoteArray[i]->startCountDown();
        }
    }

    MR::startLevelSound(this, "SE_SY_NO_SOUND_FANFARE", -1, -1, -1);
    s32 timeLeft = mTimeLimit - getNerveStep();
    if (timeLeft <= 170) {
        MR::startSystemLevelSE("SE_RS_LV_NOTE_TIMER_FAST", 0, -1);
    }
    else {
        if (timeLeft <= mTimeLimit / 2) {
            MR::startSystemLevelSE("SE_RS_LV_NOTE_TIMER_MIDDLE", 30, -1);
        }
        else {
            MR::startSystemLevelSE("SE_RS_LV_NOTE_TIMER_SLOW", 85, -1);
        }
    }

    if (isDeadAllNotes()) {
        kill();
    }
}

void NoteFairy::exeSuccessDemo() {
    if (MR::isFirstStep(this)) {
        MR::tryStartDemoRegistered(this, nullptr);
    }

    if (!MR::isTimeKeepDemoActive()) {
        setNerve(&NrvNoteFairy::NoteFairyNrvSuccess::sInstance);
    }
}

void NoteFairy::exeSuccess() {
    if (MR::isFirstStep(this)) {
        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }

        switch (mPowerStarSpawnType) {
            case 1:
                MR::requestAppearPowerStar(this, mLastGotNote->mPosition);
                break;
            case 2:
                MR::requestAppearPowerStar(this, mLastGotNote->mPosition);
                break;
            case 0:
                MR::requestAppearPowerStar(this, _8C);
                break;
        }

        setNerve(&NrvNoteFairy::NoteFairyNrvEndWait::sInstance);
    }
}

void NoteFairy::enterDemoAppear(const Nerve *pNerve, bool hasNoFrame) {
    MR::startActorCameraTargetSelf(this, mCameraInfo, -1);

    if (hasNoFrame) {
        MR::requestStartDemo(this, "出現", pNerve, nullptr);
    }
    else {
        MR::requestStartDemoWithoutCinemaFrame(this, "出現", pNerve, nullptr);
    }

    MR::requestMovementOn(MR::getSceneObj<NoteGroup*>(SceneObj_NoteGroup));
    MR::requestMovementOn(this);
}

bool NoteFairy::isDeadAllNotes() {
    for (s32 i = 0; i < mMelodyNoteNum; i++) {
        if (!MR::isDead(mNoteArray[i])) {
            return false;
        }
    }

    return true;
}

void NoteFairy::setLastGotNote(Note *pNote) {
    mLastGotNote = pNote;
}

f32 NoteFairy::getNoteCoord(s32 val) {
    if (_D1) {
        return MR::getRailPointCoord(this, val);
    }

    return val * mNoteCoord;
}

bool NoteFairy::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) {
    if (msg == 102) {
        killAllNotes();

        switch (mCompletionJingle) {
            case -1:
                break;
            case 0:
                MR::startSound(this, "SE_SY_TOTAL_COMPLETE", -1, -1);
                break;
        }

        if (mHasDemo) {
            setNerve(&NrvNoteFairy::NoteFairyNrvSuccessDemo::sInstance);
        }
        else {
            setNerve(&NrvNoteFairy::NoteFairyNrvSuccess::sInstance);
        }

        return true;
    }

    return false;
}

NoteFairy::~NoteFairy() {

}

namespace NrvNoteFairy {
    INIT_NERVE(NoteFairyNrvHide);
    INIT_NERVE(NoteFairyNrvStartAppearDemo);
    INIT_NERVE(NoteFairyNrvRailMoveStart);
    INIT_NERVE(NoteFairyNrvDemoAppear);
    INIT_NERVE(NoteFairyNrvAppearNoteBloom);
    INIT_NERVE(NoteFairyNrvAppearNote);
    INIT_NERVE(NoteFairyNrvEndWait);
    INIT_NERVE(NoteFairyNrvSuccessDemo);
    INIT_NERVE(NoteFairyNrvSuccess);
};
