#pragma once

#include "Game/LiveActor/PartsModel.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class StarPiece;

// This class may not belong in this file
class RotPartsModel : public PartsModel {
public:
    RotPartsModel(LiveActor* a1, const char* a2, const char* a3, MtxPtr a4, int a5, bool a6) : PartsModel(a1, a2, a3, a4, a5, a6) {
    }

    virtual ~RotPartsModel() {
    }
    virtual void calcAndSetBaseMtx() {
        PartsModel::calcAndSetBaseMtx();
        TRot3f mtx;
        mtx.identity();

        // makeMatrixFromRotAxesInline
        TVec3f rot(mRotation);
        rot *= PI_180;

        f32 Z = rot.z;
        f32 Y = rot.y;
        f32 X = rot.x;

        f32 cosZ = cos(Z);
        f32 cosY = cos(Y);
        f32 cosX = cos(X);
        f32 sinZ = sin(Z);
        f32 sinY = sin(Y);
        f32 sinX = sin(X);

        mtx.setXDir(cosY * cosZ, cosY * sinZ, -sinY);
        f32 asds = sinX * cosY;
        mtx.setYDir(sinX * sinY * cosZ - cosX * sinZ, sinX * sinY * sinZ + cosX * cosZ, asds);
        mtx.setZDir(cosX * cosZ * sinY + sinX * sinZ, cosX * sinZ * sinY - sinX * cosZ, cosX * cosY);

        PSMTXConcat(getBaseMtx(), mtx, getBaseMtx());
    }
};

class ItemBubble : public LiveActor {
public:
    ItemBubble(const char*);

    virtual ~ItemBubble();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeBreak();

    const TVec3f& getRotPartsPosition(int index) const {
        return _90[index]->mPosition;
    }

    StarPiece* getStarPiece(int index) const {
        return _94[index];
    }

    f32 _8C;              // 0x8C
    RotPartsModel** _90;  // 0x90
    StarPiece** _94;      // 0x94
    u16 mItemCount;       // 0x98
    u16 mItemType;        // 0x9A
    TPos3f _9C;           // 0x9C
    TPos3f _CC;           // 0xCC
    TVec3f _FC;           // 0xFC
    TVec3f _108;          // 0x108
    f32 mRailSpeed;       // 0x114
    bool mUseRail;        // 0x118
};
