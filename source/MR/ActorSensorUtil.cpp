#include "MR/ActorSensorUtil.h"
#include "LiveActor/LiveActor.h"

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

    bool isMsgPlayerSpinAttack(u32 msg)
    {
        return !(msg - 1);
    }
};