#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Enemy/KariKariDirector.hpp"

void MarioActor::initBlackHoleOut() {
    mPosRelativeToBlackHole = mPosition - mBlackHolePosition;
    
    TVec3f normalisedRelativePos(mPosRelativeToBlackHole);
    TVec3f normalisedRelativeCameraPos = mCamPos - mBlackHolePosition;
    
    MR::normalizeOrZero(&normalisedRelativeCameraPos);
    MR::normalizeOrZero(&normalisedRelativePos);
    
    Mtx rotation;
    TVec3f rotateAxis;
    
    PSVECCrossProduct(&normalisedRelativeCameraPos, &normalisedRelativePos, &rotateAxis);
    
    f32 mag = PSVECMag(&mPosRelativeToBlackHole);
    TVec3f killed;
    f32 flt = MR::vecKillElement(mCamPos - mPosition, mCamDirZ, &killed);
    flt *= mConst->getTable()->mBlackHoleFirstRadius;
    
    PSMTXRotAxisRad(rotation, &rotateAxis, atan(flt / mag));
    PSMTXMultVec(rotation, &mPosRelativeToBlackHole, &mBlackHoleRotateAxis);
    MR::normalizeOrZero(&mBlackHoleRotateAxis);
    
    damageDropThrowMemoSensor();
    MR::removeAllClingingKarikari();
}

void MarioActor::exeGameOverBlackHole2() {
    if (MR::isFirstStep(this)) {
        MR::setCubeBgmChangeInvalid();
        MR::clearBgmQueue();
        
        if (!(mBlackHole->tryStartDemoCamera()) && !(mMario->mMovementStates_HIGH_WORD >> 8 & 0x1)) {
            MR::startBlackHoleCamera("ブラックホール", mBlackHolePosition, mPosition);
        }
        
        _F44 = false;
        
        changeAnimationNonStop("ブラックホール落下");
        playEffect("共通ブラックホール");
        playSound("ブラックホール吸い込まれ", -1);
        playEffect(changeMorphString("DieBlackHole"));
        initBlackHoleOut();
        
        mMario->mMovementStates_HIGH_WORD |= 0x8;
        
        MR::startStarPointerModeDemoMarioDeath(this);
        MR::deactivateDefaultGameLayout();
    }
    
    if (getNerveStep() == 60) {
        
        if (!MR::getPlayerLeft()) {
            MR::startPlayerEvent("ゲームオーバー");
        }
        else {
            MR::startPlayerEvent("マリオ奈落");
        }
    }
    
    if (getNerveStep() == mConst->getTable()->mBlackHoleHideTime) {
        _482 = true;
        
        MR::hidePlayer();
        MR::emitEffect(this, "ブラックホール消滅");
    }
    
    f32 nervestepfloat = getNerveStep();
    f32 flt = 1.0f;
    
    if (nervestepfloat < 180.0f) {
        flt = MR::sqrt(nervestepfloat/180.0f);
    }
    
    f32 angle = mConst->getTable()->mBlackHoleRotateSpeed;
    angle = nervestepfloat * angle;
    angle = flt * angle;
    
    if (angle > mConst->getTable()->mBlackHoleRotateLimit) {
        angle = mConst->getTable()->mBlackHoleRotateLimit;
    }
    
    Mtx rotationMatrix;
    PSMTXRotAxisRad(rotationMatrix,&mBlackHoleRotateAxis, angle);
    PSMTXMultVec(rotationMatrix, &mPosRelativeToBlackHole, &mPosRelativeToBlackHole);
    
    TVec3f camDirZNegate;
    JMathInlineVEC::PSVECNegate(&mCamDirZ, &camDirZNegate);
    MR::vecBlendSphere(mBlackHoleRotateAxis, camDirZNegate, &mBlackHoleRotateAxis, 0.01f);
    
    f32 distChangeFactor = 180 - getNerveStep();
    
    if (distChangeFactor < 0.0f) {
        distChangeFactor = 0.0f;
    }
    
    f32 newDistToBlackHole = PSVECMag(&mPosRelativeToBlackHole) * distChangeFactor/(1+distChangeFactor);
    
    mPosRelativeToBlackHole.setLength(newDistToBlackHole);
    
    f32 scale = getNerveStep() * mConst->getTable()->mBlackHoleScaleSpeed;
    scale = 1-scale;
    
    if (scale < mConst->getTable()->mBlackHoleScaleLimit) {
        scale = mConst->getTable()->mBlackHoleScaleLimit;
    }
    
    mScale.set(scale);
    
    mPosition = mBlackHolePosition + mPosRelativeToBlackHole;
    
    mVelocity.zero();
}
