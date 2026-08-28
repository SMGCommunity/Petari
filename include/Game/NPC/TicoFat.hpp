#pragma once

#include "Game/NPC/NPCActor.hpp"

class ActorCameraInfo;
class BenefitItemObj;
class FullnessMeter;
class SpinDriverShootPath;

class TicoFat : public NPCActor {
public:
    TicoFat(const char*);

    /* 0x08 */ virtual ~TicoFat();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x78 */ virtual void shootStarPiece();
    /* 0x7C */ virtual void receiveStarPiece(s32);
    /* 0x80 */ virtual void startEat();
    /* 0x84 */ virtual bool enableAppear();
    /* 0x88 */ virtual void initStarPieceSaveData(const JMapInfoIter&);
    /* 0x8C */ virtual void addStarPieceSaveData(s32);
    /* 0x90 */ virtual void disappear(bool);
    /* 0x94 */ virtual void appearInformation() const;

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
