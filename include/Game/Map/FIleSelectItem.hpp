#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/FileSelectIconID.hpp"
#include "Game/System/NerveExecutor.hpp"

class FileSelectItem;
class FileSelectModel;
class MiiFaceParts;
class FileSelectNumber;
class FileSelectItemDelegatorBase;

namespace FileSelectItemSub {
    class ScaleController : public NerveExecutor {
    public:
        ScaleController();

        virtual ~ScaleController();

        void exeToSmall();
        void exeToBig();
        void exeSmall();
        void exeBig();

        f32 _8;
    };

    class BlinkController : public NerveExecutor {
    public:
        BlinkController(FileSelectItem*);

        virtual ~BlinkController();

        void exeOpen();
        void exeShut();
        void exeSleep();
        void exeBlink();
        void shut();
        void open();
        void sleep();

        FileSelectItem* mItem;  // 0x08
        s32 _C;
        s32 _10;
    };
};  // namespace FileSelectItemSub

class FileSelectItem : public LiveActor {
public:
    FileSelectItem(s32, bool, const FileSelectIconID&, const char*);

    virtual ~FileSelectItem();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void control();

    bool isNew() const;
    bool isExist() const;
    void format();
    void change(const FileSelectIconID&, bool);
    void forceChange(const FileSelectIconID&, bool);
    void invalidateSelect();
    void validateSelect();
    void appearIndex();
    void disappearIndex();
    void copyIconID(FileSelectIconID*);
    void setSelectDelegator(FileSelectItemDelegatorBase*);
    void onPointing();
    void offPointing();
    void validateRotate();
    void turnToFront(s32);
    void exeFormat();
    void exeChangeFellow();
    void exeChangeMii();
    void createNew();
    void createFellows();
    void createMii();
    void createNumber();
    void updatePointing();
    void updateRotate();
    void playPointedME();
    void playPointedNotUsingME();
    void appearFellowModel();
    void killAllModels();
    void emitOpen();
    void emitVanish();
    void emitCopy();
    void emitCompleteEffect();
    void deleteCompleteEffect();

    void exeExistWait();
    void exeNewWait();

    bool _8C;
    PartsModel* mPlanetMapObj;  // 0x90
    FileSelectIconID* mIconID;  // 0x94
    FileSelectModel** mModels;  // 0x98
    MiiFaceParts* mFaceParts;   // 0x9C
    FileSelectNumber* _A0;
    TMtx34f _A4;
    TMtx34f _D4;
    TMtx34f _104;
    TVec3f _134;
    u32 _140;
    u8 _144;
    bool mIsInvalidateSelect;  // 0x145
    u8 _146;
    u8 _147;
    FileSelectItemSub::ScaleController* mScaleCtrl;  // 0x148
    FileSelectItemSub::BlinkController* mBlinkCtrl;  // 0x14C
    FileSelectItemDelegatorBase* mDelegator;         // 0x150
    u8 _154;
    u8 _155;
    u8 _156;
    u8 _157;
    TVec2f _158;
    f32 _160;
    bool mIsInvalidRotate;  // 0x164
    u8 _165;
    u8 _166;
    u8 _167;
    u32 _168;
    u32 _16C;
};
