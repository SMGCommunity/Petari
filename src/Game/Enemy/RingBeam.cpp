#include "Game/Enemy/RingBeam.hpp"
#include "Game/LiveActor/HitSensor.hpp"

//GX function that is included as a local symbol for some reason
extern "C"{
void GXPosition3f32(f32,f32,f32);
}


RingBeamShadowDrawer::RingBeamShadowDrawer(const LiveActor * unk0) : ShadowVolumeDrawer("影描画[リングビーム]"){
    _1c = unk0;
    _20 = 0.0f;
}

void RingBeamShadowDrawer::drawShape() const{
    TPos3f MtxList[32];
    TPos3f baseMtxCopy;
    Mtx mtx;
    TVec3f temp;
    TVec3f temp2;
    TVec3f temp3;
    TVec3f listXDirScaledPos;
    TVec3f XDirScaled;
    TVec3f listXDirScaledNeg;
    TVec3f negYDirScaled;
    TVec3f XDir;
    TVec3f YDir;
    TVec3f Trans;
    TVec3f listXDir;
    TVec3f listYDir;
    float f1 = 0.2026834f;

    JMath::gekko_ps_copy12(&baseMtxCopy,_1c->getBaseMtx());
    baseMtxCopy.getXDir(XDir);
    baseMtxCopy.getYDir(YDir);
    baseMtxCopy.getTrans(Trans);
    PSMTXRotAxisRad(mtx,&YDir,f1);
    XDirScaled.scale(_20, XDir);
    float f2 = 0.0f;
    for(int i = 0; i < 32; i++){
        JMathInlineVEC::PSVECAdd(&Trans,&XDirScaled,&temp);
        MR::calcGravityVector(_1c,temp,&temp2,nullptr,0);
        if(MR::normalizeOrZero(XDirScaled,&temp3)){
            JMath::gekko_ps_copy12(&MtxList[i],&baseMtxCopy);
        } else if(MR::isSameDirection(-temp2,temp3,0.01f)){
            MR::makeMtxUpSide(&MtxList[i],-temp2,-YDir);
        } else if(temp2.dot(YDir) < f2){
            MR::makeMtxUpSide(&MtxList[i],-temp2,temp3);
        } else{
            MR::makeMtxUpSide(&MtxList[i],-temp2,-temp3);
        }
        // almost certainly an inline
        MtxList[i][0][3] = temp.x;
        MtxList[i][1][3] = temp.y;
        MtxList[i][2][3] = temp.z;

        PSMTXMultVec(mtx, &XDirScaled, &XDirScaled);
    }
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 64);
    for(int i = 0; i < 32; i++){
        MtxList[i].getXDir(listXDir);
        MtxList[i].getTrans(Trans);
        listXDirScaledNeg.scale(-5.0f,listXDir);
        listXDirScaledPos.scale(5.0f,listXDir);
        JMathInlineVEC::PSVECAdd(&Trans,&listXDirScaledNeg,&temp);
        JMathInlineVEC::PSVECAdd(&Trans,&listXDirScaledPos,&temp2);
        GXPosition3f32(temp2.x,temp2.y,temp2.z);
        GXPosition3f32(temp.x,temp.y,temp.z);
    }
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 64);
    for(int i = 0; i < 32; i++){
        MtxList[i].getXDir(listXDir);
        MtxList[i].getYDir(listYDir);
        MtxList[i].getTrans(Trans);

        listXDirScaledNeg.scale(-5.0f,listXDir);
        listXDirScaledPos.scale(5.0f,listXDir);
        negYDirScaled.scale(500.0f,-listYDir);
        JMathInlineVEC::PSVECAdd(&Trans,&listXDirScaledNeg,&temp);
        JMathInlineVEC::PSVECAdd(&Trans,&listXDirScaledPos,&temp2);
        JMathInlineVEC::PSVECAdd(&temp,&negYDirScaled,&temp);
        JMathInlineVEC::PSVECAdd(&temp2,&negYDirScaled,&temp2);
        GXPosition3f32(temp.x,temp.y,temp.z);
        GXPosition3f32(temp2.x,temp2.y,temp2.z);
    }
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 64);
    for(int i = 0; i < 32; i++){
        MtxList[i].getXDir(listXDir);
        MtxList[i].getYDir(listYDir);
        MtxList[i].getTrans(Trans);

        listXDirScaledNeg.scale(-5.0f,listXDir);
        negYDirScaled.scale(500.0f,-listYDir);
        JMathInlineVEC::PSVECAdd(&Trans,&listXDirScaledNeg,&temp);
        JMathInlineVEC::PSVECAdd(&Trans,&listXDirScaledNeg,&temp2);
        JMathInlineVEC::PSVECAdd(&temp2,&negYDirScaled,&temp2);
        GXPosition3f32(temp.x,temp.y,temp.z);
        GXPosition3f32(temp2.x,temp2.y,temp2.z);
    }
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 64);
    for(int i = 0; i < 32; i++){
        MtxList[i].getXDir(listXDir);
        MtxList[i].getYDir(listYDir);
        MtxList[i].getTrans(Trans);

        listXDirScaledPos.scale(5.0f,listXDir);
        negYDirScaled.scale(500.0f,-listYDir);
        JMathInlineVEC::PSVECAdd(&Trans,&listXDirScaledPos,&temp);
        JMathInlineVEC::PSVECAdd(&Trans,&listXDirScaledPos,&temp2);
        JMathInlineVEC::PSVECAdd(&temp2,&negYDirScaled,&temp2);
        GXPosition3f32(temp2.x,temp2.y,temp2.z);
        GXPosition3f32(temp.x,temp.y,temp.z);
    }
}

void RingBeamShadowDrawer::loadModelDrawMtx() const{
  
  GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
  GXSetCurrentMtx(0);
  GXClearVtxDesc();
  GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
  GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
};

bool RingBeamShadowDrawer::isDraw() const{
    return MR::isValidDraw(_1c);
}


RingBeam::RingBeam(const char * pName, LiveActor * arg1, bool arg2, bool arg3) : LiveActor(pName){
    _8c = arg1;
    mSpeed = 20.0f;
    mLife = 100;
    _9c = arg2;
    _9d = arg3;
    _a0 = nullptr;
    _a4 = nullptr;
}

void RingBeam::setSpeed(f32 speed){
    mSpeed = speed;
}


void RingBeam::setLife(s32 life){
    mLife = life;
}

void RingBeam::init(const JMapInfoIter &rIter){
        TPos3f mtx;
    MR::initDefaultPos(this,rIter);
    initModelManagerWithAnm("JumpBeamerBeam",nullptr,false);
    MR::connectToSceneIndirectEnemy(this);
    initHitSensor(1);
    MR::addHitSensorEye(this,"Beam",8,0.0f,TVec3f(0.0f,0.0f,0.0f));
    initEffectKeeper(1,nullptr,false);
    initSound(2,true);
    mSoundObject->setTrans(&_cc);
    MR::invalidateClipping(this);
    initNerve(&NrvRingBeam::RingBeamNrvSpread::sInstance);
    
    initPos(this);
    JMath::gekko_ps_copy12(&mtx,this->getBaseMtx());
    mtx.getZDir(_a8);
    mtx.getTrans(_c0);
    mtx.getTrans(_cc);
    if(_9d){
    _a4 = new ModelObj("影モデル","JumpBeamerBeamShadow",nullptr,28,42,8,0);
    _a4->initWithoutIter();
    _a4->mPosition.set(this->mPosition);
    _a4->mRotation.set(this->mRotation);
    _a4->mScale.set(this->mScale);
    }else{
    _a0 = new RingBeamShadowDrawer(this);
    }
    mBloomModel = MR::createModelObjBloomModel("リングビームブルーム","JumpBeamerBeamBloom", getBaseMtx());
    MR::invalidateClipping(mBloomModel);
    makeActorDead();
}

void RingBeam::appear(){
    LiveActor::appear();
    mBloomModel->appear();
}

void RingBeam::kill(){
    setNerve(&NrvRingBeam::RingBeamNrvSpread::sInstance);
    initPos(_8c);
    setRadius(0.0f);
    LiveActor::kill();
    mBloomModel->kill();
}

void RingBeam::startBrk(const char * pchar){
    MR::startBrk(this,pchar);
    if(_a4){
        MR::startBrk(_a4,pchar);
    }
}

void RingBeam::setRadius(f32 radius){
    TVec3f Ydir;
    TVec3f temp2;
    TVec3f temp6;
    
    if (radius >= 2000.0f){
        radius = 2000.0f;
    } 
    if(_a0){
        _a0->_20 = radius;
    }
    getSensor("Beam")->mRadius = radius; 
    char * str = "Spread";
    MR::startBckNoInterpole(this,str);
    MR::startBckNoInterpole(mBloomModel,str);
    radius = (radius/2000.0f)*MR::getBckFrameMax(this);
    MR::setBckFrameAndStop(this,radius);
    MR::setBckFrameAndStop(mBloomModel,radius);
    if(_a4){
        Triangle triangle;
        MR::extractMtxYDir(_8c->getBaseMtx(),&Ydir);
        Ydir.scale(1000.0f);
        TVec3f temp = _b4;
        temp.scale(500.0f);
        MR::getFirstPolyOnLineToMap(&temp2,&triangle,_c0,temp);
        TVec3f temp3 = _b4;
        temp3.scale(10.0f);
        temp2 -= temp3;
        TVec3f temp4 = _8c->mPosition;
        JMathInlineVEC::PSVECAdd(&temp4,&Ydir,&temp4);
        TVec3f temp5 = _8c->mPosition;
        temp5 -=Ydir;
        MR::calcPerpendicFootToLine(&temp6,temp2,temp4,temp5);
        _a4->mPosition.set(temp6);
        MR::startBckNoInterpole(_a4,str);
        TVec3f temp7 = temp2;
        temp7 -= temp6;
        f32 length = temp7.length();
        if(0.0f <= length && length < 2000.0f){
            MR::setBckFrameAndStop(_a4,(length/2000.0f)*MR::getBckFrameMax(_a4));
        }
    }
}


void RingBeam::initPos(const LiveActor * actor){
    TRot3f actorBaseMtxCopy;
    TVec3f baseMtxYdir;
    JMath::gekko_ps_copy12(&actorBaseMtxCopy,actor->getBaseMtx());
    actorBaseMtxCopy.getYDir(baseMtxYdir);
    mPosition.set(actor->mPosition+ baseMtxYdir.scaleInline(75.0f));
}



void RingBeam::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    TRot3f mtx;
    TVec3f temp;
    TVec3f temp2;
    TVec3f sensordiff = pReceiver->mPosition;
    sensordiff -= pSender->mPosition;
    MR::makeMtxTR((MtxPtr)&mtx, this);
    //this is 100% certainly an inline
    f32 x = mtx.mMtx[0][1];
    f32 y = mtx.mMtx[1][1];
    f32 z = mtx.mMtx[2][1];
    temp.set<f32>(x, y, z);
    MR::vecKillElement(sensordiff, temp, &temp2);
    temp2.setLength(pSender->mRadius);
    
    TVec3f temp3 = temp2;
    temp3 -= sensordiff;
    f32 length = temp3.length();
    f32 receiverRadius = pReceiver->mRadius;

    if (!(length > 5.0f + receiverRadius) && MR::isSensorPlayerOrRide(pReceiver)) {
        MR::sendMsgEnemyAttackElectric(pReceiver, pSender);
    }
}


bool RingBeam::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    return false;
}



// has a bunch of stack problems and wrong operators 
// due to TVec jank.
void RingBeam::exeSpread(){
    TPos3f mtx;
    TVec3f temp;
    TVec3f ec;
    TVec3f temp7;
    TVec3f temp12;
    TVec3f temp13;
    TVec3f temp15;
    TVec3f temp3;
    if(MR::isFirstStep(this)){
        if(!MR::isPowerStarGetDemoActive()){
            MR::startSound(this,"SE_EM_JUMPRING_APPEAR",-1,-1);
        }
        char * str = "Spread";
        MR::startBckNoInterpole(this,str);
        MR::startBckNoInterpole(mBloomModel,str);
        startBrk("Erase");
        MR::setBrkFrameAndStop(this,0.0f);
        if(_a4){
            MR::setBrkFrameAndStop(_a4,0.0f);
        }
        MR::extractMtxZDir(_8c->getBaseMtx(),&_a8);
        MR::extractMtxYDir(_8c->getBaseMtx(),&temp);
        TVec3f temp2 = temp;
        temp2.scale(75.0f);
        temp3 = _8c->mPosition;
        JMathInlineVEC::PSVECAdd(&temp3, &temp2,&temp3);
        _c0.set(temp3);
        MR::calcGravityVector(this,_c0,&_b4,nullptr,0);
    }
    if(_9c){
        MR::calcGravityVector(this,_c0 + _a8.scaleInline(mSpeed),&_b4,nullptr,0);
        TVec3f temp6 = -_b4;
        MR::makeMtxUpFront(&mtx,temp6,_a8);
        mtx.getZDir(_a8);
        MR::extractMtxYDir(_8c->getBaseMtx(),&temp7);
        TVec3f temp8(_8c->mPosition);
        JMathInlineVEC::PSVECAdd(&temp8,&temp7,&temp8);
        TVec3f temp9(_8c->mPosition);
        temp9 -= temp7;
        MR::calcPerpendicFootToLine(&ec,_c0,temp8,temp9);
        TVec3f temp10(ec);
        mPosition = temp10;
    }else{
        ec = mPosition;
    }
    JMathInlineVEC::PSVECAdd(&_c0,&(_a8.scaleInline(mSpeed)),&_c0);
    TVec3f temp11 = _c0;
    temp11 -= ec;
    setRadius(PSVECMag(&temp11));
     if(!MR::isPowerStarGetDemoActive()){
        temp12 = MR::getCamPos();
        temp12 -= ec;
        if(!MR::isNearZero(temp12,0.001f)){
            f32 radius;
            f32 dot;
            f32 float2;
            MR::extractMtxYDir(_8c->getBaseMtx(),&temp13);
            dot = temp12.dot(temp13);
            TVec3f temp14 = temp13.scaleInline(dot);
            temp15 = temp12;
            temp15 -= temp14;
            MR::normalizeOrZero(&temp15);
            MR::normalizeOrZero(&temp12);
            float2 = 1.0f-__fabsf(temp13.dot(temp12));
            radius = getSensor("Beam")->mRadius;
            _cc = ec + temp15.scaleInline(radius).scaleInline(float2);
        }else{
            _cc = ec;
        }
        MR::startLevelSound(this,"SE_EM_LV_RINGBEAM_MOVE",-1,-1,-1);

     }
     if(MR::isStep(this,mLife - (s32)MR::getBrkFrameMax(this))){
        startBrk("Erase");
     }
     if(MR::isGreaterEqualStep(this,mLife)){
        kill();
     }
}

namespace NrvRingBeam {
    INIT_NERVE(RingBeamNrvSpread);
    void RingBeamNrvSpread::execute(Spine *pSpine) const{
        RingBeam *pActor = (RingBeam*)pSpine->mExecutor;
        pActor->exeSpread();
    }

}

RingBeam::~RingBeam(){};
RingBeamShadowDrawer::~RingBeamShadowDrawer(){};
