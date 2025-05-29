#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/HitSensorKeeper.hpp"
#include "Game/LiveActor/HitSensorInfo.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/LiveActor/LiveActorGroupArray.hpp"
#include "Game/Util.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace MR {
    HitSensor* addHitSensor(LiveActor *pActor, const char *pSensorName, u32 sensorType, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, sensorType, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorBinder(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x61, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorTransferableBinder(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x62, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorPriorBinder(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x63, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorRide(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x9, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorMapObj(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x46, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorMapObjPress(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x76, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorMapObjSimple(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x47, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorMapObjMoveCollision(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x48, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorEnemy(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x14, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorEnemySimple(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x15, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorEnemyAttack(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x16, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorNpc(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x5, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorEye(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x7F, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorPush(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x80, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorPosBinder(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f *pPos, const TVec3f &a6) {
        return pActor->mSensorKeeper->addPos(pSensorName, 0x61, sensorGroupSize, radius, pActor, pPos, a6);
    }

    HitSensor* addHitSensorPosRide(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f *pPos, const TVec3f &a6) {
        return pActor->mSensorKeeper->addPos(pSensorName, 0x9, sensorGroupSize, radius, pActor, pPos, a6);
    }

    HitSensor* addHitSensorPosMapObj(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f *pPos, const TVec3f &a6) {
        return pActor->mSensorKeeper->addPos(pSensorName, 0x46, sensorGroupSize, radius, pActor, pPos, a6);
    }

    HitSensor* addHitSensorPosEye(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f *pPos, const TVec3f &a6) {
        return pActor->mSensorKeeper->addPos(pSensorName, 0x7F, sensorGroupSize, radius, pActor, pPos, a6);
    }

    HitSensor* addHitSensorMtx(LiveActor *pActor, const char *pSensorName, u32 sensorType, u16 sensorGroupSize, f32 radius, MtxPtr mtx, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, sensorType, sensorGroupSize, radius, pActor, mtx, a6);
    }

    HitSensor* addHitSensorMtxRide(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, MtxPtr mtx, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x9, sensorGroupSize, radius, pActor, mtx, a6);
    }

    HitSensor* addHitSensorMtxMapObj(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, MtxPtr mtx, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x46, sensorGroupSize, radius, pActor, mtx, a6);
    }

    HitSensor* addHitSensorMtxEnemy(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, MtxPtr mtx, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x14, sensorGroupSize, radius, pActor, mtx, a6);
    }

    HitSensor* addHitSensorMtxEnemyAttack(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, MtxPtr mtx, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x16, sensorGroupSize, radius, pActor, mtx, a6);
    }

    HitSensor* addHitSensorMtxNpc(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, MtxPtr mtx, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x5, sensorGroupSize, radius, pActor, mtx, a6);
    }

    HitSensor* addHitSensorMtxAnimal(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, MtxPtr mtx, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x7, sensorGroupSize, radius, pActor, mtx, a6);
    }

    HitSensor* addHitSensorAtJoint(LiveActor *pActor, const char *pSensorName, const char *pJointName, u32 sensorType, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, sensorType, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointRide(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x9, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointMapObj(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x46, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointMapObjSimple(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x47, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointEnemy(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x14, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointEnemySimple(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x15, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointEnemyAttack(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x16, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointNpc(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x5, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointEye(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x7F, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorCallback(LiveActor *pActor, const char *pSensorName, u32 sensorType, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, sensorType, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackBinder(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x61, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackPriorBinder(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x63, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackRide(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x9, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackMapObj(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x46, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackMapObjSimple(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x47, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackEnemy(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x14, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackEnemyAttack(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x16, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackEye(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x7F, sensorGroupSize, radius, pActor);
    }

    HitSensor* addBodyMessageSensorReceiver(LiveActor *pActor){
        TVec3f zerovec = TVec3f(0.0f,0.0f,0.0f);
        return pActor->mSensorKeeper->add("body",(u32)0x84,(u16)0,0.0f,pActor,zerovec);
    }

    HitSensor* addBodyMessageSensorMapObj(LiveActor *pActor){
        TVec3f zerovec = TVec3f(0.0f,0.0f,0.0f);
        return pActor->mSensorKeeper->add("body",(u32)0x46,(u16)0,0.0f,pActor,zerovec);
    }

    HitSensor* addBodyMessageSensorMapObjPress(LiveActor *pActor){
        TVec3f zerovec = TVec3f(0.0f,0.0f,0.0f);
        return pActor->mSensorKeeper->add("body",(u32)0x76,(u16)0,0.0f,pActor,zerovec);
    }

    HitSensor* addBodyMessageSensorMapObjMoveCollision(LiveActor *pActor){
        TVec3f zerovec = TVec3f(0.0f,0.0f,0.0f);
        return pActor->mSensorKeeper->add("body",(u32)0x48,(u16)0,0.0f,pActor,zerovec);
    }

    HitSensor* addBodyMessageSensorEnemy(LiveActor *pActor){
        TVec3f zerovec = TVec3f(0.0f,0.0f,0.0f);
        return pActor->mSensorKeeper->add("body",(u32)0x14,(u16)0,0.0f,pActor,zerovec);
    }

    HitSensor* addBodyMessageSensorReceiver(LiveActor *pActor, const char *pSensorName){
        TVec3f zerovec = TVec3f(0.0f,0.0f,0.0f);
        return pActor->mSensorKeeper->add(pSensorName,(u32)0x84,(u16)0,0.0f,pActor,zerovec);
    }

    HitSensor* addBodyMessageSensorMapObj(LiveActor *pActor, const char *pSensorName){
        TVec3f zerovec = TVec3f(0.0f,0.0f,0.0f);
        return pActor->mSensorKeeper->add(pSensorName,(u32)0x46,(u16)0,0.0f,pActor,zerovec);
    }

    HitSensor* addBodyMessageSensorMapObjMoveCollision(LiveActor *pActor, const char *pSensorName){
        TVec3f zerovec = TVec3f(0.0f,0.0f,0.0f);
        return pActor->mSensorKeeper->add(pSensorName,(u32)0x48,(u16)0,0.0f,pActor,zerovec);
    }

    HitSensor* addBodyMessageSensorEnemy(LiveActor *pActor, const char *pSensorName){
        TVec3f zerovec = TVec3f(0.0f,0.0f,0.0f);
        return pActor->mSensorKeeper->add(pSensorName,(u32)0x14,(u16)0,0.0f,pActor,zerovec);
    }

    bool tryUpdateHitSensorsAll(LiveActor *pActor){
        if(pActor->mSensorKeeper){
            pActor->mSensorKeeper->update();
            return 1;
        }
        return 0;
    }

    void UpdateHitSensorsAll(LiveActor *pActor){
        pActor->mSensorKeeper->update();
    }

    bool isSensorType(const HitSensor *pSensor, u32 sensorType){
        return pSensor->isType(sensorType);
    }

    HitSensor * getSensorWithIndex(LiveActor *pActor, int index){
        return pActor->mSensorKeeper->getNthSensorInfo(index)->mSensor;
    }

    HitSensor * getTaking(const LiveActor *pActor){
        if(pActor->mSensorKeeper){
            return pActor->mSensorKeeper->_C;
        }
        return 0;
    }

    HitSensor * getTaken(const LiveActor *pActor){
        if(pActor->mSensorKeeper){
            return pActor->mSensorKeeper->_10;
        }
        return 0;
    }

    void setSensorPos(HitSensor *pSensor, const TVec3f &Pos){
        pSensor->mPosition.set(Pos);
    }

    void setSensorOffset(LiveActor *pActor, const char *pSensorName, const TVec3f &offset){
        pActor->mSensorKeeper->getSensorInfo(pSensorName)->_C.setPS(offset);
    }

    void setSensorRadius(LiveActor *pActor, const char *pSensorName, f32 radius){
        pActor->mSensorKeeper->getSensorInfo(pSensorName)->mSensor->mRadius = radius;
    }

    void setHitSensorApart(HitSensor *pSensor1, HitSensor *pSensor2){
        if((getTaking(pSensor1->mActor) == pSensor2) || (getTaken(pSensor2->mActor) == pSensor1)){
            pSensor1->mActor->mSensorKeeper->_C = nullptr;
            pSensor2->mActor->mSensorKeeper->_10 = nullptr;
        }else{
            pSensor1->mActor->mSensorKeeper->_10 = nullptr;
            pSensor2->mActor->mSensorKeeper->_C = nullptr;
        }
    }

    void validateHitSensors(LiveActor *pActor) {
        if (pActor->mSensorKeeper) {
            pActor->mSensorKeeper->validate();
        }
    }

    void invalidateHitSensors(LiveActor *pActor) {
        if (pActor->mSensorKeeper) {
            pActor->mSensorKeeper->invalidate();
        }
    }

    void validateHitSensors(LiveActor *pActor, const char *pSensorName) {
        pActor->mSensorKeeper->getSensor(pSensorName)->validate();
    }

    void invalidateHitSensors(LiveActor *pActor, const char *pSensorName) {
        pActor->mSensorKeeper->getSensor(pSensorName)->invalidate();
    }

    bool isValidHitSensor(LiveActor *pActor, const char *pSensorName){
        return pActor->mSensorKeeper->getSensor(pSensorName)->mValidByHost;
    }

    void clearHitSensors(LiveActor *pActor){
        if(pActor->mSensorKeeper) {
            pActor->mSensorKeeper->clear();
        }
    }

    // the exact same as getSensorWithIndex
    HitSensor * getSensor(LiveActor *pActor, int index){
        return pActor->mSensorKeeper->getNthSensorInfo(index)->mSensor;
    }

    LiveActor * getSensorHost(const HitSensor *pSensor) {
        return pSensor->mActor;
    }

    bool isSensor(const HitSensor *pSensor, const char *pSensorName) {
        return (pSensor == pSensor->mActor->getSensor(pSensorName));
    }

    bool isSensorPlayer(const HitSensor *pSensor) {
        return pSensor->isType(0x1);
    }

    bool isSensorBinder(const HitSensor *pSensor) {
        return pSensor->isType(0x61);
    }

    bool isSensorRide(const HitSensor *pSensor) {
        if (0x8 < pSensor->mSensorType && pSensor->mSensorType < 0x12) {
            return true;
        }
        
        return false;
    }

    bool isSensorPlayerOrRide(const HitSensor *pSensor) {
        return isSensorPlayer(pSensor) || isSensorRide(pSensor);
    }

    bool isSensorEnemy(const HitSensor *pSensor) {
        if(0x13 < pSensor->mSensorType && pSensor->mSensorType < 0x44){
            return true;
        }
        else{
            
            return false;
        }
    }

    bool isSensorEnemyAttack(const HitSensor *pSensor) {
        return pSensor->isType(0x16);
    }

    bool isSensorNPC(const HitSensor *pSensor) {
        // this just returns pSensor->mSensorType == 0x5?
        if(0x4 < pSensor->mSensorType && pSensor->mSensorType < 0x6){
            return true;
        }
        else{
            
            return false;
        }
    }

    bool isSensorMapObj(const HitSensor *pSensor) {
        if (pSensor->mSensorType > 0x45 && pSensor->mSensorType < 0x5F) {
            return true;
        }

        return false;
    }

    bool isSensorAutoRush(const HitSensor *pSensor) {
        if (pSensor->mSensorType > 0x60 && pSensor->mSensorType < 0x6E) {
            return true;
        }

        return false;
    }

    bool isSensorRush(const HitSensor *pSensor) {
        if (pSensor->mSensorType > 0x6F && pSensor->mSensorType < 0x74) {
            return true;
        }

        return false;
    }

    bool isSensorPressObj(const HitSensor *pSensor) {
        if (pSensor->mSensorType > 0x75 && pSensor->mSensorType < 0x77) {
            return true;
        }

        return false;
    }

    bool isSensorEye(const HitSensor *pSensor) {
        return pSensor->isType(0x7F);
    }

    bool isSensorPush(const HitSensor *pSensor) {
        return pSensor->isType(0x80);
    }

    bool isSensorItem(const HitSensor *pSensor) {
        if (pSensor->isType(0x4A) || pSensor->isType(0x4B) || pSensor->isType(0x4D)) {
            return true;
        }

        return false;
    }

    bool tryGetItem(HitSensor *pSender, HitSensor *pReceiver) {
        return pReceiver->receiveMessage(0x87, pSender);
    }

    TVec3f* getSensorPos(const HitSensor *pSensor) {
        return (TVec3f*)&pSensor->mPosition;
    }

   void calcSensorDirection(TVec3f *result, const HitSensor *pSensor1, const HitSensor *pSensor2){
        TVec3f direction = pSensor2->mPosition;
        JMathInlineVEC::PSVECSubtract(&direction,&pSensor1->mPosition,&direction);
        result->set(direction);
    }

    void calcSensorDirectionNormalize(TVec3f *result, const HitSensor *pSensor1, const HitSensor *pSensor2){
        TVec3f direction = pSensor2->mPosition;
        JMathInlineVEC::PSVECSubtract(&direction,&pSensor1->mPosition,&direction);
        result->set(direction);
        normalizeOrZero(result);
    }

    void calcSensorHorizonNormalize(TVec3f *result, const TVec3f &vec,const HitSensor *pSensor1, const HitSensor *pSensor2){
        TVec3f direction = pSensor2->mPosition;
        JMathInlineVEC::PSVECSubtract(&direction,&pSensor1->mPosition,&direction);
        result->rejection(direction,vec);
        normalizeOrZero(result);
    }

    HitSensor* getMessageSensor(){
        return MR::getSceneObj<LiveActor*>(0x9)->getSensor(0);
    }

    bool sendArbitraryMsg(u32 msg, HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(msg, pSender);
    }

    bool sendMsgPush(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x29, pSender);
    }

    bool sendMsgPlayerTrample(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(2, pSender);
    }

    bool sendMsgPlayerPunch(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(1, pSender);
    }

    bool sendMsgJump(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x2C, pSender);
    }

    bool sendMsgTouchJump(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x2D, pSender);
    }

    bool sendMsgTaken(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x1F, pSender);
    }

    bool sendMsgKick(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x2B, pSender);
    }

    bool sendMsgAwayJump(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x2F, pSender);
    }

    bool sendMsgEnemyAttackMsgToDir(u32 msg, HitSensor *pReceiver, HitSensor *pSender, const TVec3f &rDir) {
        TVec3f curSenderPos(pSender->mPosition);
        JMathInlineVEC::PSVECSubtract(&pReceiver->mPosition, &rDir, &pSender->mPosition);
        bool receive = pReceiver->receiveMessage(msg, pSender);
        setSensorPos(pSender,curSenderPos);
        return receive;
    }

    bool sendMsgEnemyAttackFlipWeak(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x4D, pSender);
    }

    bool sendMsgEnemyAttackFlipWeakJump(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x4E, pSender);
    }

    bool sendMsgEnemyAttackFlip(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x50, pSender);
    }

    bool sendMsgEnemyAttackFlipToDir(HitSensor *pReceiver, HitSensor *pSender, const TVec3f &rDir) {
        return sendMsgEnemyAttackMsgToDir(0x50, pReceiver, pSender, rDir);
    }

    bool sendMsgEnemyAttackFlipJump(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x4F, pSender);
    }

    bool sendMsgEnemyAttackFlipRot(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x51, pSender);
    }

    bool sendMsgEnemyAttackFlipMaximum(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x52, pSender);
    }

    bool sendMsgEnemyAttackFlipMaximumToDir(HitSensor *pReceiver, HitSensor *pSender, const TVec3f &rDir) {
        return sendMsgEnemyAttackMsgToDir(0x52, pReceiver, pSender, rDir);
    }

    bool sendMsgEnemyAttack(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x53, pSender);
    }

    bool sendMsgEnemyAttackStrong(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x54, pSender);
    }

    bool sendMsgEnemyAttackStrongToDir(HitSensor *pReceiver, HitSensor *pSender, const TVec3f &rDir) {
        return sendMsgEnemyAttackMsgToDir(0x54, pReceiver, pSender, rDir);
    }

    bool sendMsgEnemyAttackFire(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x58, pSender);
    }

    bool sendMsgEnemyAttackFireStrong(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x59, pSender);
    }

    bool sendMsgEnemyAttackElectric(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x5A, pSender);
    }

    bool sendMsgEnemyAttackFreeze(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x5D, pSender);
    }

    bool sendMsgEnemyAttackHeatBeam(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x5B, pSender);
    }

    bool sendMsgEnemyAttackExplosion(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x56, pSender);
    }

    bool sendMsgEnemyAttackCounterSpin(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x60, pSender);
    }

    bool sendMsgEnemyAttackCounterHipDrop(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x61, pSender);
    }

    bool sendMsgLockOnStarPieceShoot(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0xE, pSender);
    }

    bool sendMsgStarPieceAttack(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0xC, pSender);
    }

    bool sendMsgStarPieceGift(HitSensor *pReceiver, HitSensor *pSender, u32 msg) {
        return pReceiver->receiveMessage(msg + 0xE, pSender);
    }

    bool sendMsgEnemyAttackMaximum(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x55, pSender);
    }

    bool sendMsgEnemyAttackMaximumToDir(HitSensor *pReceiver, HitSensor *pSender, const TVec3f &rDir) {
        return sendMsgEnemyAttackMsgToDir(0x55, pReceiver, pSender, rDir);
    }

    bool sendMsgEnemyAttackToBindedSensor(LiveActor *pActor, HitSensor *pSender) {
       return sendMsgToBindedSensor(0x53,pActor,pSender);
    }

    bool sendMsgEnemyAttackExplosionToBindedSensor(LiveActor *pActor, HitSensor *pSender) {
       return sendMsgToBindedSensor(0x56,pActor,pSender);
    }

    bool sendSimpleMsgToActor(u32 msg, LiveActor *pActor){
        return pActor->receiveMessage(msg, getMessageSensor(), getMessageSensor());
    }

    bool sendMsgToBindedSensor(u32 msg, LiveActor *pActor, HitSensor *pSender){
        const u32 arraysize = 0x20;
        HitInfo* info[arraysize];
        bool sent;
        HitSensor* sensor;
        u32 length = pActor->mBinder->copyPlaneArrayAndSortingSensor(&info[0], arraysize);
        if(length == 0){
            return false;
        }
        sent = false;
        sensor = info[0]->mParentTriangle.mSensor;
        sent |= sensor->receiveMessage(msg,pSender);
        for(int i = 1; i < length; i++){
            if(info[i]->mParentTriangle.mSensor == sensor){
                continue;
            }
            sensor = info[i]->mParentTriangle.mSensor;
            sent |= sensor->receiveMessage(msg,pSender);
        }

        return sent;
    }

    bool sendMsgToBindedSensor(u32 msg, HitSensor *pSender){
        return sendMsgToBindedSensor(msg,pSender->mActor,pSender);
    }

    bool sendMsgToGroundSensor(u32 msg, HitSensor *pSender){
        if(!MR::isBindedGround(pSender->mActor)){
            return false;
        }
        return MR::getGroundSensor(pSender->mActor)->receiveMessage(msg,pSender);
    }

    bool sendMsgToWallSensor(u32 msg, HitSensor *pSender){
        if(!MR::isBindedWall(pSender->mActor)){
            return false;
        }
        return MR::getWallSensor(pSender->mActor)->receiveMessage(msg,pSender);
    }

    bool sendMsgStartDemo(LiveActor *pActor){
        // this function is getting inlined here where it shouldn't be
        return sendSimpleMsgToActor(0x6f,pActor);
    }

    bool sendMsgToEnemyAttackBlow(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x62,pSender);
    }

    bool sendMsgToEnemyAttackTrample(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x63,pSender);
    }

    bool sendMsgToEnemyAttackBlowOrTrample(HitSensor *pReceiver, HitSensor *pSender, f32 flt) {
        TVec3f posDifference = pReceiver->mPosition;
        JMathInlineVEC::PSVECSubtract(&posDifference,&pSender->mPosition,&posDifference);
        MR::normalizeOrZero(&posDifference);
        if(flt < pSender->mActor->mGravity.dot(posDifference))
            return sendMsgToEnemyAttackTrample(pReceiver,pSender);
        else{
            return sendMsgToEnemyAttackBlow(pReceiver,pSender);
        }
    }

    bool sendMsgToEnemyAttackShockWave(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x64,pSender);
    }

    void sendMsgToAllLiveActor(u32 msg, LiveActor *pActor){
        AllLiveActorGroup* Grp = getAllLiveActorGroup();
        for(int i = 0; i < Grp->getObjectCount(); i++){
            LiveActor* GrpActor = Grp->getActor(i);
            if(!MR::isDead(GrpActor) && GrpActor != pActor){
                sendSimpleMsgToActor(msg,GrpActor);
            }
        }
    }

    void sendMsgToGroupMember(u32 msg, LiveActor *pActor, HitSensor *pSender, const char *pSensorName){
        MsgSharedGroup* grp = (MsgSharedGroup*)MR::getGroupFromArray(pActor);
        if(grp != nullptr){
            grp->sendMsgToGroupMember(msg,pSender,pSensorName);
        }else{
            pActor->receiveMessage(msg,pSender,pActor->getSensor(pSensorName));
        }
    }

    void sendMsgExplosionToNearActor(HitSensor *pSender, f32 flt){
        HitSensor * sensor;
        for(u16 i = 0; i < pSender->mSensorCount; i++){
            sensor = pSender->mSensors[i];
            if(flt < 0.0f || MR::isNear(pSender,sensor,flt)){
                sensor->receiveMessage(0x56,pSender);
            }
        }
    }

    bool isInSpinStormRange(u32 msg, HitSensor *pReceiver, HitSensor *pSender, f32 flt){
        if(msg != 0x33){
            return false;
        }
        //?? double not?
        bool isNotNear = !MR::isNear(pReceiver, pSender, flt);
        return !isNotNear;
    }

    bool receiveItemShowMsg(u32 msg, HitSensor *pReceiver, HitSensor *pSender){
        if(msg == 0x8a && MR::isDead(pSender->mActor)){
            pSender->mActor->makeActorAppeared();
            return true;
        }
        return false;
    }

    bool receiveItemHideMsg(u32 msg, HitSensor *pReceiver, HitSensor *pSender){
        if(msg == 0x8b && !MR::isDead(pSender->mActor)){
            pSender->mActor->makeActorDead();
            return true;
        }
        return false;
    }

    //These 3 functions appear to mention members of the Binder class
    //that do not exist with how the class is mapped out currently.
    HitSensor *getGroundSensor(const LiveActor *pActor){
        // return pActor->mBinder->_44
    }

    HitSensor *getRoofSensor(const LiveActor *pActor){
        // return pActor->mBinder->_164
    }

    HitSensor *getWallSensor(const LiveActor *pActor){
        // return pActor->mBinder->_d4
    }

    bool isMsgPlayerHitAll(u32 msg) {
        return msg == 0x1 || msg == 0x5 || msg == 0x6 || msg == 8 || msg == 10 || msg == 11;
    }

    bool isMsgPlayerSpinAttack(u32 msg) {
        return msg == 0x1;
    }

    bool isMsgPlayerTrample(u32 msg) {
        return  msg == 0x2;    
    }

    bool isMsgPlayerHipDrop(u32 msg) {
        return  msg == 0x3;
    }

    bool isMsgPlayerHipDropFloor(u32 msg) {
        return  msg == 0x4;    
    }

    bool isMsgPlayerUpperPunch(u32 msg) {
        return  msg == 0x5;
    }

    bool isMsgPlayerKick(u32 msg) {
        return  msg == 0x2b;
    }

    bool isMsgJetTurtleAttack(u32 msg) {
        return  msg == 0x6;
    }

    bool isMsgFireBallAttack(u32 msg) {
        return  msg == 0x8;
    }

    bool isMsgSearchlightAttack(u32 msg) {
        return  msg == 0x9;
    }

    bool isMsgFreezeAttack(u32 msg) {
        return  msg == 0xa;
    }

    bool isMsgInvincibleAttack(u32 msg) {
        return  msg == 0xb;
    }

    bool isMsgInvalidHit(u32 msg) {
        return  msg == 0x1d;
    }

    bool isMsgAutoRushBegin(u32 msg) {
        return  msg == 0x92;
    }

    bool isMsgRushBegin(u32 msg) {
        return  msg == 0x91;
    }

    bool isMsgUpdateBaseMtx(u32 msg) {
        return  msg == 0xa1;
    }

    bool isMsgRushCancel(u32 msg) {
        return  msg == 0x93;
    }

    bool isMsgIsRushTakeOver(u32 msg) {
        return  msg == 0x98;
    }

    bool isMsgFloorTouch(u32 msg) {
        return  msg == 0xb4;
    }

    bool isMsgWallTouch(u32 msg) {
        return  msg == 0xb5;
    }

    bool isMsgCeilTouch(u32 msg) {
        return  msg == 0xb6;
    }

    bool isMsgItemGet(u32 msg) {
        return  msg == 0x87;
    }

    bool isMsgItemPull(u32 msg) {
        return  msg == 0x89;
    }

    bool isMsgItemShow(u32 msg) {
        return  msg == 0x8a;
    }

    bool isMsgItemHide(u32 msg) {
        return  msg == 0x8b;
    }

    bool isMsgItemStartMove(u32 msg) {
        return  msg == 0x8c;
    }

    bool isMsgItemEndMove(u32 msg) {
        return  msg == 0x8d;
    }

    bool isMsgInhaleBlackHole(u32 msg) {
        return  msg == 0x73;
    }

    bool isMsgEnemyAttack(u32 msg) {
        return  msg == 0x53;
    }

    bool isMsgEnemyAttackFire(u32 msg) {
        return  msg == 0x58;
    }

    bool isMsgEnemyAttackFireStrong(u32 msg) {
        return  msg == 0x59;
    }

    bool isMsgEnemyAttackElectric(u32 msg) {
        return  msg == 0x5a;
    }

    bool isMsgExplosionAttack(u32 msg) {
        return  msg == 0x56;
    }

    bool isMsgToEnemyAttackBlow(u32 msg) {
        return  msg == 0x62;
    }

    bool isMsgToEnemyAttackTrample(u32 msg) {
        return  msg == 0x63;
    }

    bool isMsgToEnemyAttackShockWave(u32 msg) {
        return  msg == 0x64;
    }

    bool isMsgSpinStormRange(u32 msg) {
        return  msg == 0x33;
    }

    bool isMsgTutorialStart(u32 msg) {
        return  msg == 0xe6;
    }

    bool isMsgTutorialNext(u32 msg) {
        return  msg == 0xe7;
    }

    bool isMsgTutorialPrev(u32 msg) {
        return  msg == 0xe8;
    }

    bool isMsgTutorialPass(u32 msg) {
        return  msg == 0xe9;
    }

    bool isMsgTutorialOmit(u32 msg) {
        return  msg == 0xeb;
    }

    bool isMsgRaceReady(u32 msg) {
        return  msg == 0xec;
    }

    bool isMsgRaceStart(u32 msg) {
        return  msg == 0xed;
    }

    bool isMsgRaceReset(u32 msg) {
        return  msg == 0xef;
    }

    bool isMsgLockOnStarPieceShoot(u32 msg) {
        return  msg == 0xe;
    }

    bool isMsgBallDashWall(u32 msg) {
        return  msg == 0x39;
    }

    bool isMsgBallDashGround(u32 msg) {
        return  msg == 0x3a;
    }

    bool isMsgStartPowerStarGet(u32 msg) {
        return  msg == 0x74;
    }

    bool isMsgTouchPlantItem(u32 msg) {
        return  msg == 0xf3;
    }

    bool isMsgHitmarkEmit(u32 msg) {
        return  msg == 0x1c;
    }

    bool isMsgStarPieceAttack(u32 msg) {
        return  msg == 0xc;
    }

    bool isMsgStarPieceReflect(u32 msg) {
        return  msg == 0xd;
    }

    bool isMsgStarPieceGift(u32 msg) {
        return  msg >= 0xf && msg <= 0x19;
    }
    
    u32 getNumStarPieceGift(u32 num){
        return num - 14;
    }
    // one float regswap away from matching, probably an inline of some sort
    void calcPosBetweenSensors(TVec3f *pVec, const HitSensor *pSensor1, const HitSensor *pSensor2, f32 flt){
        TVec3f posDifference = pSensor2->mPosition;
        JMathInlineVEC::PSVECSubtract(&posDifference,&pSensor1->mPosition,&posDifference);
        MR::normalizeOrZero(&posDifference);
        f32 x = PSVECDistance(&pSensor1->mPosition,&pSensor2->mPosition);
        f32 ya = pSensor2->mRadius;
        f32 yb = pSensor1->mRadius;
        f32 dist = yb + ya -x;
        dist *= 0.5f;
        pVec->set<f32>(posDifference);
        pVec->x *= dist + pSensor1->mRadius + flt;
        pVec->y *= dist + pSensor1->mRadius + flt;
        pVec->z *= dist + pSensor1->mRadius + flt;
        JMathInlineVEC::PSVECAdd2(pVec,&pSensor1->mPosition,pVec);
    }

    bool tryForceKillIfMsgStartPowerStarGet(LiveActor *pActor, u32 msg){
        if(msg == 0x74){
            if(isExistEffectKeeper(pActor)){
                forceDeleteEffectAll(pActor);
            }
            pActor->makeActorDead();
            return true;
        }
        return false;
    }

    void addBodyMessageSensor(LiveActor *pActor, u32 sensorType){
        TVec3f zero = TVec3f(0.0f,0.0f,0.0f);
        pActor->mSensorKeeper->add("body",sensorType,0,0.0f,pActor,zero);
    }
};
