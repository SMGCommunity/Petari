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

        mtx.setRotateDegree(mRotation);

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

    /* 0x8C */ f32 _8C;
    /* 0x90 */ RotPartsModel** _90;
    /* 0x94 */ StarPiece** _94;
    /* 0x98 */ u16 mItemCount;
    /* 0x9A */ u16 mItemType;
    /* 0x9C */ TPos3f _9C;
    /* 0xCC */ TPos3f _CC;
    /* 0xFC */ TVec3f _FC;
    /* 0x108 */ TVec3f _108;
    /* 0x114 */ f32 mRailSpeed;
    /* 0x118 */ bool mUseRail;
};
