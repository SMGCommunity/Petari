#pragma once

#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/BenefitItemLifeUp.hpp"
#include "Game/MapObj/BenefitItemObj.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class FullnessMeter;
class SpinDriverShootPath;

class TicoFat : public NPCActor {
public:
    TicoFat(const char*);

    virtual ~TicoFat();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual void shootStarPiece();
    virtual void receiveStarPiece(s32);
    virtual void startEat();
    virtual bool enableAppear();
    virtual void initStarPieceSaveData(const JMapInfoIter&);
    virtual void addStarPieceSaveData(s32);
    virtual void disappear(bool);
    virtual void appearInformation() const;

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    const char* getActionName(const char*);
    void setCameraParam();
    bool eventFunc(u32);
    f32 calcScale();
    void setScale(f32);
    void setMessage(s32);
    bool tryMetamorphosis();
    void emitScreenEffect();
    void updateScreenEffect();
    s32 getDanceSeTranspose() const;
    void startAbsorbSound();
    void updateGuidance();
    void updatePointing();
    void updateGoods();
    void exeReaction();
    void exePrep();
    void exeWait();
    void exePoint();
    void exeEat();
    void exeChem();
    void exeTest();
    void exeFullness();
    void exeMeta();
    void exeDemo();
    void exeFly();
    void exeWipeOut();
    void exeWipeIn();
    void exeInfo();
    void exeAfter();

    ActorCameraInfo* mCameraInfo;     // 0x15C
    BenefitItemObj* mItem;            // 0x160
    FullnessMeter* mMeter;            // 0x164
    SpinDriverShootPath* mShootPath;  // 0x168
    TalkMessageCtrl* _16C;
    u8 _170;
    bool mStartEat;  // 0x171
    u8 _172;
    u8 _173;
    s32 _174;
    s32 _178;
    TPos3f _17C;
    TPos3f _1AC;
    s32 _1DC;
    s32 _1E0;
    s32 _1E4;
    s32 mCurrentFed;  // 0x1E8
    s32 _1EC;
    u32 _1F0;
    s32 _1F4;
    f32 _1F8;
    f32 _1FC;
    u32 _200;
    s32 _204;
};
