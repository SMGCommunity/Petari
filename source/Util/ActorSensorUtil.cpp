#include "Util/ActorSensorUtil.h"
#include "Util/SceneUtil.h"
#include "LiveActor/LiveActor.h"
#include "JMath/JMath.h"

namespace MR
{
    void addHitSensor(LiveActor *pActor, const char *pSensorName, u32 msg, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, msg, unk1, unk2, pActor, unk3);
    }

    void addHitSensorBinder(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, 0x61, unk1, unk2, pActor, unk3);
    }

    void addHitSensorTransferableBinder(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, 0x62, unk1, unk2, pActor, unk3);
    }

    void addHitSensorPriorBinder(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, 0x63, unk1, unk2, pActor, unk3);
    }

    void addHitSensorRide(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, 0x5, unk1, unk2, pActor, unk3);
    }

    void addHitSensorMapObj(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, 0x46, unk1, unk2, pActor, unk3);
    }

    void addHitSensorMapObjPress(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, 0x76, unk1, unk2, pActor, unk3);
    }

    void addHitSensorMapObjSimple(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, 0x47, unk1, unk2, pActor, unk3);
    }

    void addHitSensorMapObjMoveCollision(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, 0x48, unk1, unk2, pActor, unk3);
    }

    void addHitSensorEnemy(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, 0x14, unk1, unk2, pActor, unk3);
    }

    void addHitSensorEnemySimple(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, 0x15, unk1, unk2, pActor, unk3);
    }

    void addHitSensorEnemyAttack(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, 0x16, unk1, unk2, pActor, unk3);
    }

    void addHitSensorNpc(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, 0x5, unk1, unk2, pActor, unk3);
    }

    void addHitSensorEye(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, 0x7F, unk1, unk2, pActor, unk3);
    }

    void addHitSensorPush(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, const JGeometry::TVec3<f32> &unk3)
    {
        pActor->mSensorKeeper->add(pSensorName, 0x80, unk1, unk2, pActor, unk3);
    }

    void addHitSensorPosBinder(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, JGeometry::TVec3<f32> *unk3, const JGeometry::TVec3<f32> &unk4)
    {
        pActor->mSensorKeeper->addPos(pSensorName, 0x61, unk1, unk2, pActor, unk3, unk4);
    }

    void addHitSensorPosRide(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, JGeometry::TVec3<f32> *unk3, const JGeometry::TVec3<f32> &unk4)
    {
        pActor->mSensorKeeper->addPos(pSensorName, 0x9, unk1, unk2, pActor, unk3, unk4);
    }

    void addHitSensorPosMapObj(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, JGeometry::TVec3<f32> *unk3, const JGeometry::TVec3<f32> &unk4)
    {
        pActor->mSensorKeeper->addPos(pSensorName, 0x46, unk1, unk2, pActor, unk3, unk4);
    }

    void addHitSensorPosEye(LiveActor *pActor, const char *pSensorName, u16 unk1, f32 unk2, JGeometry::TVec3<f32> *unk3, const JGeometry::TVec3<f32> &unk4)
    {
        pActor->mSensorKeeper->addPos(pSensorName, 0x7F, unk1, unk2, pActor, unk3, unk4);
    }

    bool isSensorPlayer(const HitSensor *pSensor, const char*pSensorName)
    {
        return pSensor->isType(1);
    }

    bool isSensorBinder(const HitSensor *pSensor, const char*pSensorName)
    {
        return pSensor->isType(0x61);
    }

    bool sendArbitraryMsg(u32 msg, HitSensor *pHit1, HitSensor *pHit2)
    {
        return  pHit1->recieveMessage(msg, pHit2);
    }

    bool sendMsgPush(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x29, pHit2);
    }

    bool sendMsgPlayerTrample(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x2, pHit2);
    }

    bool sendMsgPlayerPunch(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x1, pHit2);
    }

    bool sendMsgJump(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x2C, pHit2);
    }

    bool sendMsgTouchJump(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x2D, pHit2);
    }

    bool sendMsgTaken(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x1F, pHit2);
    }

    bool sendMsgKick(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x2B, pHit2);
    }

    bool sendMsgAwayJump(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x2F, pHit2);
    }

    bool sendMsgEnemyAttackMsgToDir(u32 msg, HitSensor *pHit1, HitSensor *pHit2, const JGeometry::TVec3<f32> &dir)
    {
        JGeometry::TVec3<f32> _8(pHit2->_4);       
        JMathInlineVEC::PSVECSubtract((Vec *)&pHit1->_4, (Vec *)&dir, (Vec *)&pHit2->_4);

        bool ret = pHit1->recieveMessage(msg, pHit2);
        pHit2->_4.set(_8);

        return ret;
    }

    bool sendMsgEnemyAttackFlipWeak(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x4D, pHit2);
    }

    bool sendMsgEnemyAttackFlipWeakJump(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x4E, pHit2);
    }

    bool sendMsgEnemyAttackFlip(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x50, pHit2);
    }

    bool sendMsgEnemyAttackFlipToDir(HitSensor *pHit1, HitSensor *pHit2, const JGeometry::TVec3<f32> &dir)
    {
        return sendMsgEnemyAttackMsgToDir(0x50, pHit1, pHit2, dir);
    }
    
    bool sendMsgEnemyAttackFlipJump(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x4F, pHit2);
    }

    bool sendMsgEnemyAttackFlipRot(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x51, pHit2);
    }

    bool sendMsgEnemyAttackFlipMaximum(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x52, pHit2);
    }

    bool sendMsgEnemyAttackFlipMaximumToDir(HitSensor *pHit1, HitSensor *pHit2, const JGeometry::TVec3<f32> &dir)
    {
        return sendMsgEnemyAttackMsgToDir(0x52, pHit1, pHit2, dir);
    }

    bool sendMsgEnemyAttack(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x53, pHit2);
    }

    bool sendMsgEnemyAttackStrong(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x54, pHit2);
    }

    bool sendMsgEnemyAttackStrongToDir(HitSensor *pHit1, HitSensor *pHit2, const JGeometry::TVec3<f32> &dir)
    {
        return sendMsgEnemyAttackMsgToDir(0x54, pHit1, pHit2, dir);
    }

    bool sendMsgEnemyAttackFire(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x58, pHit2);
    }

    bool sendMsgEnemyAttackFireStrong(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x59, pHit2);
    }

    bool sendMsgEnemyAttackElectric(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x5A, pHit2);
    }

    bool sendMsgEnemyAttackFreeze(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x5D, pHit2);
    }

    bool sendMsgEnemyAttackHeatBeam(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x5B, pHit2);
    }

    bool sendMsgEnemyAttackExplosion(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x56, pHit2);
    }

    bool sendMsgEnemyCounterSpin(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x60, pHit2);
    }

    bool sendMsgEnemyAttackCounterHipDrop(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x61, pHit2);
    }

    bool sendMsgLockOnStarPieceShoot(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0xE, pHit2);
    }

    bool sendMsgStarPieceAttack(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0xC, pHit2);
    }

    bool sendMsgStarPieceGift(HitSensor *pHit1, HitSensor *pHit2, u32 msg)
    {
        return pHit1->recieveMessage(msg + 14, pHit2);
    }

    bool sendMsgEnemyattackMaximum(HitSensor *pHit1, HitSensor *pHit2)
    {
        return pHit1->recieveMessage(0x55, pHit2);
    }

    bool sendMsgEnemyAttackMaximumToDir(HitSensor *pHit1, HitSensor *pHit2, const JGeometry::TVec3<f32> &dir)
    {
        return sendMsgEnemyAttackMsgToDir(0x55, pHit1, pHit2, dir);
    }

    bool isMsgPlayerHitAll(u32 msg)
    {
        u32 playerMsg = msg -1;
        bool ret = false;

        if (playerMsg <= 0xA && ((1 << playerMsg) & 0x6B1) != 0)
        {
            ret = true;
        }

        return ret;
    }

    bool isMsgPlayerSpinAttack(u32 msg)
    {
        return !(msg - 1);
    }

    bool isMsgPlayerTrample(u32 msg)
    {
        return !(msg - 2);
    }

    bool isMsgPlayerHipDrop(u32 msg)
    {
        return !(msg - 3);
    }
    
    bool isMsgPlayerHipDropFloor(u32 msg)
    {
        return !(msg -4);
    }

    bool isMsgPlayerUpperPunch(u32 msg)
    {
        return !(msg -5);
    }

    bool isMsgPlayerKick(u32 msg)
    {
        return !(msg -0x2B);
    }

    bool isMsgJetTurtleAttack(u32 msg)
    {
        return !(msg -6);
    }

    bool isMsgFireBallAttack(u32 msg)
    {
        return !(msg -8);
    }

    bool isMsgSearchlightAttack(u32 msg)
    {
        return !(msg -9);
    }

    bool isMsgFreezeAttack(u32 msg)
    {
        return !(msg -0xA);
    }

    bool isMsgInvincibleAttack(u32 msg)
    {
        return !(msg -0xB);
    }

    bool isMsgInvalidHit(u32 msg)
    {
        return !(msg -0x1D);
    }

    bool isMsgAutoRushBegin(u32 msg)
    {
        return !(msg -0x92);
    }

    bool isMsgRushBegin(u32 msg)
    {
        return !(msg -0x91);
    }

    bool isMsgUpdateBaseMtx(u32 msg)
    {
        return !(msg -0xA1);
    }

    bool isMsgRushCancel(u32 msg)
    {
        return !(msg -0x93);
    }

    bool isMsgRushTakeOver(u32 msg)
    {
        return !(msg -0x98);
    }

    bool isMsgFloorTouch(u32 msg)
    {
        return !(msg -0xB4);
    }

    bool isMsgWallTouch(u32 msg)
    {
        return !(msg -0xB5);
    }

    bool isMsgCeilTouch(u32 msg)
    {
        return !(msg -0xB6);
    }

    bool isMsgItemGet(u32 msg)
    {
        return !(msg -0x87);
    }

    bool isMsgItemPull(u32 msg)
    {
        return !(msg -0x89);
    }

    bool isMsgItemShow(u32 msg)
    {
        return !(msg -0x8A);
    }

    bool isMsgItemHide(u32 msg)
    {
        return !(msg -0x8B);
    }

    bool isMsgItemStartMove(u32 msg)
    {
        return !(msg -0x8C);
    }

    bool isMsgItemEndMove(u32 msg)
    {
        return !(msg -0x8D);
    }

    bool isMsgInhaleBlackHole(u32 msg)
    {
        return !(msg -0x73);
    }

    bool isMsgEnemyAttack(u32 msg)
    {
        return !(msg -0x53);
    }

    bool isMsgEnemyAttackFire(u32 msg)
    {
        return !(msg -0x58);
    }

    bool isMsgEnemyAttackFireStrong(u32 msg)
    {
        return !(msg -0x59);
    }

    bool isMsgEnemyAttackElectric(u32 msg)
    {
        return !(msg -0x5A);
    }

    bool isMsgEnemyAttackExplosion(u32 msg)
    {
        return !(msg -0x56);
    }

    bool isMsgEnemyAttackBlow(u32 msg)
    {
        return !(msg -0x62);
    }

    bool isMsgEnemyAttackTrample(u32 msg)
    {
        return !(msg -0x63);
    }

    bool isMsgEnemyAttackShockWave(u32 msg)
    {
        return !(msg -0x64);
    }

    bool isMsgSpinStormRange(u32 msg)
    {
        return !(msg -0x33);
    }

    bool isMsgTutorialStart(u32 msg)
    {
        return !(msg -0xE6);
    }

    bool isMsgTutorialNext(u32 msg)
    {
        return !(msg -0xE7);
    }

    bool isMsgTutorialPrev(u32 msg)
    {
        return !(msg -0xE8);
    }

    bool isMsgTutorialPass(u32 msg)
    {
        return !(msg -0xE9);
    }

    bool isMsgTutorialOmit(u32 msg)
    {
        return !(msg -0xEB);
    }

    bool isMsgRaceReady(u32 msg)
    {
        return !(msg -0xEC);
    }

    bool isMsgRaceStart(u32 msg)
    {
        return !(msg -0xED);
    }

    bool isMsgRaceReset(u32 msg)
    {
        return !(msg -0xEF);
    }

    bool isMsgLockOnStarPieceShoot(u32 msg)
    {
        return !(msg -0xE);
    }

    bool isMsgBallDashWall(u32 msg)
    {
        return !(msg -0x39);
    }

    bool isMsgBallDashGround(u32 msg)
    {
        return !(msg -0x3A);
    }

    bool isMsgStartPowerStarGet(u32 msg)
    {
        return !(msg -0x74);
    }

    bool isMsgTouchPlantItem(u32 msg)
    {
        return !(msg -0xF3);
    }

    bool isMsgHitmarkEmit(u32 msg)
    {
        return !(msg -0x1C);
    }

    bool isMsgStarPieceAttack(u32 msg)
    {
        return !(msg -0xC);
    }

    bool isMsgStarPieceReflect(u32 msg)
    {
        return !(msg -0xD);
    }

    bool isMsgStarPieceGift(u32 msg)
    {
        bool ret = false;

        if (msg >= 0xF && msg <= 0x19)
        {
            ret = true;
        }

        return ret;
    }
};