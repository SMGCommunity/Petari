#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Enemy/KariKariDirector.hpp"

void MarioActor::initBlackHoleOut(){
    
    mPosRelativeToBlackHole = mPosition - mBlackHolePosition;
    TVec3f F68copy(mPosRelativeToBlackHole);
    TVec3f dif = mCamPos - mBlackHolePosition;
    MR::normalizeOrZero(&dif);
    MR::normalizeOrZero(&F68copy);
    Mtx trans;
    TVec3f cross;
    PSVECCrossProduct(&dif, &F68copy, &cross);
    f32 mag = PSVECMag(&mPosRelativeToBlackHole);
    TVec3f killed;
    f32 flt = MR::vecKillElement(mCamPos - mPosition,mCamDirZ,&killed);
    flt *= mConst->getTable()->mBlackHoleFirstRadius;
    PSMTXRotAxisRad(trans, &cross, atan(flt / mag));
    PSMTXMultVec(trans,&mPosRelativeToBlackHole,&mBlackHoleRotateAxis);
    MR::normalizeOrZero(&mBlackHoleRotateAxis);
    damageDropThrowMemoSensor();
    MR::removeAllClingingKarikari();
}

void MarioActor::exeGameOverBlackHole2(){
    if(MR::isFirstStep(this)){
        MR::setCubeBgmChangeInvalid();
        MR::clearBgmQueue();
        if(!(mBlackHole->tryStartDemoCamera()) && !(mMario->mMovementStates_HIGH_WORD)){
            MR::startBlackHoleCamera("ブラックホール",mBlackHolePosition,mPosition);
        }
        _F44 = false;
        changeAnimationNonStop("ブラックホール落下");
        playEffect("共通ブラックホール");
        playSound("ブラックホール吸い込まれ",-1);
        playEffect(changeMorphString("DieBlackHole"));
        initBlackHoleOut();
        mMario->mMovementStates_HIGH_WORD |= 0x8;
        MR::startStarPointerModeDemoMarioDeath(this);
        MR::deactivateDefaultGameLayout();
    }   
    if(getNerveStep() == 0x3c){
        if(!MR::getPlayerLeft()){
            MR::startPlayerEvent("ゲームオーバー");
        }else{
            MR::startPlayerEvent("マリオ奈落");
        }
    }
    if(getNerveStep() == mConst->getTable()->mBlackHoleHideTime){
        _482 = true;
        MR::hidePlayer();
        MR::emitEffect(this, "ブラックホール消滅");
    }
    f32 nervestepfloat = getNerveStep();
    f32 flt = 1.0f;
    if(nervestepfloat < 180.0f){
        flt = MR::sqrt(nervestepfloat/180.0f);
    }
    f32 angle = mConst->getTable()->mBlackHoleRotateSpeed;
    angle = nervestepfloat * angle;
    angle = flt * angle;
    if(angle > mConst->getTable()->mBlackHoleRotateLimit){
        angle = mConst->getTable()->mBlackHoleRotateLimit;
    }
    Mtx rotationMatrix;
    PSMTXRotAxisRad(rotationMatrix,&mBlackHoleRotateAxis,angle);
    PSMTXMultVec(rotationMatrix,&mPosRelativeToBlackHole,&mPosRelativeToBlackHole);
    TVec3f vec;
    JMathInlineVEC::PSVECNegate(&mCamDirZ,&vec);
    MR::vecBlendSphere(mBlackHoleRotateAxis,vec,&mBlackHoleRotateAxis,0.01f);
    f32 distChangeFactor = 0xb4 - getNerveStep();
    if(distChangeFactor < 0.0f){
        distChangeFactor = 0.0f;
    }
    f32 NewDistToBlackHole = PSVECMag(&mPosRelativeToBlackHole) * distChangeFactor/(1+distChangeFactor);
    mPosRelativeToBlackHole.setLength(NewDistToBlackHole);
    f32 scale = getNerveStep() * mConst->getTable()->mBlackHoleScaleSpeed;
    scale = 1-scale;
    if(scale < mConst->getTable()->mBlackHoleScaleLimit){
        scale = mConst->getTable()->mBlackHoleScaleLimit;
    }
    mScale.set(scale);
    mPosition = mBlackHolePosition + mPosRelativeToBlackHole;
    mVelocity.zero();
}
