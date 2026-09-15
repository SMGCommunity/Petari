#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/System/NerveExecutor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class FileSelectIconID;
class FileSelectItem;
class FileSelectItemDelegatorBase;
class FileSelectModel;
class FileSelectNumber;
class MiiFaceParts;
class PartsModel;

namespace FileSelectItemSub {
    class ScaleController : public NerveExecutor {
    public:
        ScaleController();

        void exeToSmall();
        void exeToBig();
        void exeSmall();
        void exeBig();

        f32 _8;
    };

    class BlinkController : public NerveExecutor {
    public:
        BlinkController(FileSelectItem*);

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

    /* 0x08C */ bool _8C;
    /* 0x090 */ PartsModel* mPlanetMapObj;
    /* 0x094 */ FileSelectIconID* mIconID;
    /* 0x098 */ FileSelectModel** mModels;
    /* 0x09C */ MiiFaceParts* mFaceParts;
    /* 0x0A0 */ FileSelectNumber* _A0;
    /* 0x0A4 */ TPos3f _A4;
    /* 0x0D4 */ TMtx34f _D4;
    /* 0x104 */ TMtx34f _104;
    /* 0x134 */ TVec3f _134;
    /* 0x140 */ s32 _140;
    /* 0x144 */ u8 _144;
    /* 0x145 */ bool mIsInvalidateSelect;
    /* 0x146 */ u8 _146;
    /* 0x147 */ u8 _147;
    /* 0x148 */ FileSelectItemSub::ScaleController* mScaleCtrl;
    /* 0x14C */ FileSelectItemSub::BlinkController* mBlinkCtrl;
    /* 0x150 */ FileSelectItemDelegatorBase* mDelegator;
    /* 0x154 */ u8 _154;
    /* 0x155 */ u8 _155;
    /* 0x156 */ u8 _156;
    /* 0x157 */ u8 _157;
    /* 0x158 */ TVec2f _158;
    /* 0x160 */ f32 _160;
    /* 0x164 */ bool mIsInvalidRotate;
    /* 0x165 */ u8 _165;
    /* 0x166 */ u8 _166;
    /* 0x167 */ u8 _167;
    /* 0x168 */ s32 _168;
    /* 0x16C */ s32 _16C;
};
