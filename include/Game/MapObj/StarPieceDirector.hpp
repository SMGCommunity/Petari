#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class HitSensor;
class StarPieceShooter;
class StarPieceDirector;
class StarPiece;

namespace MR {
    StarPieceDirector* getStarPieceDirector();

    void createStarPiece();
    StarPiece* getDeadStarPiece();

    void resetChasingStarPiece();
    bool isResetChasingStarPiece();

    bool gotByPlayer();

    bool giftStarPieceToTarget(HitSensor*, u32);

    void incNumStarPieceGettable(s32);
    void incNumStarPieceNewed();

};  // namespace MR

class StarPieceHostInfo;
class StarPieceReceiverInfo;

class StarPieceDirector : public LiveActorGroup {
public:
    StarPieceDirector(const char*);

    virtual void movement();

    void declare(const NameObj*, s32);
    void declareReceiveNum(const NameObj*, s32);
    s32 getDeclareRemnantStarPieceCount(const NameObj*);
    s32 getDeclareRemnantStarPieceCountNotExist(const NameObj*);
    bool hopPiece(const NameObj*, const TVec3f&, const TVec3f&);
    bool appearPiece(const NameObj*, const TVec3f&, s32, f32, f32, bool, bool);
    bool appearPieceToDirection(const NameObj*, const TVec3f&, const TVec3f&, s32, f32, f32, bool, bool);
    bool gotByPlayer();
    void updateSound();
    void initCSDelay();
    void initCSSound();
    bool giftToTarget(HitSensor*, u32);
    StarPiece* getDeadStarPiece();
    TVec3f calcPosCameraShoot(s32);
    void clearGotCountReceiver(const NameObj*);
    StarPieceHostInfo* findHostInfo(const NameObj*);
    StarPieceReceiverInfo* findReceiverInfo(const NameObj*);
    void calcShootGoalUsingPointingDepth();

    /* 0x18 */ u32 mNumStarPieceNewed;
    /* 0x1C */ u32 mNumStarPieceGettable;
    /* 0x20 */ s32 mColorIndex;
    /* 0x24 */ s32 mNumStarPiecesLaunched;

    /* 0x28 */ StarPieceHostInfo* mHostInfoArray[0x200];

    /* 0x828 */ u32 mNewHostInfoIndex;

    /* 0x82C */ StarPieceReceiverInfo* mReceiverInfoArray[0x40];
    /* 0x92C */ u32 mNewReceiverInfoIndex;
    /* 0x930 */ bool mResetChasingStarPiece;
    /* 0x934 */ StarPieceShooter* mStarPieceShooters[2];

    // only fist 16 entries ever get used
    /* 0x93C */ bool mGetSoundArray[0x20];

    /* 0x95C */ s32 mSoundIndex;
    /* 0x960 */ bool mQueueNewGetSound;
};

class StarPieceShooter : public LiveActor {
public:
    StarPieceShooter(s32, const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeLockOn();

    bool shoot();
    bool tryShoot();

    void calcShootGoalUsingPointingDepth();

    f32 _8C;
    HitSensor* _90;
    f32 _94;
    HitSensor* _98;
    f32 _9C;
    s32 _A0;
    s32 _A4;
    TVec3f _A8;
    TVec3f _B4;
};
