#pragma once

#include "Game/NameObj/NameObj.hpp"

class CameraInfo;
class CounterLayoutController;
class InformationMessage;
class MarioMeter;
class MarioSubMeter;
class MissLayout;
class NoteCounter;
class OneUpBoard;
class PurpleCoinCounter;
class YesNoLayout;

class GameSceneLayoutHolder : public NameObj {
public:
    /// @brief Creates a new `GameSceneLayoutHolder`.
    GameSceneLayoutHolder();

    virtual void movement();

    void createPurpleCoinCounter();
    PurpleCoinCounter* getPurpleCoinCounter() const;
    void initLifeCount(u32);
    void setLifeCount(u32);
    void setOxygenRatio(f32);
    void setBeePowerRatio(f32);
    void changeLifeMeterModeGround();
    void changeLifeMeterModeWater();
    void changeLifeMeterModeBee();
    void requestOneUp(s32);
    void requestPowerUpHPMeter();
    void requestForceAppearHPMeter();
    void startMiss();
    bool isMissAnimEnd() const;
    void activateDefaultGameLayout();
    void deactivateDefaultGameLayout();
    void invalidateStatusLayout();

    /* 0x0C */ CameraInfo* mCameraInfo;
    /* 0x10 */ InformationMessage* mInformationMessage;
    /* 0x14 */ MarioMeter* mMarioMeter;
    /* 0x18 */ MarioSubMeter* mMarioSubMeter;
    /* 0x1C */ MissLayout* mMissLayout;
    /* 0x20 */ NoteCounter* mNoteCounter;
    /* 0x24 */ OneUpBoard* mOneUpBoard;
    /* 0x28 */ YesNoLayout* mYesNoLayout;
    /* 0x2C */ PurpleCoinCounter* mPurpleCoinCounter;
    /* 0x30 */ bool _30;
    /* 0x31 */ bool _31;
    /* 0x34 */ CounterLayoutController* mCounterLayoutCtrl;
    /* 0x38 */ bool mIsActiveDefaultGameLayout;
};

namespace MR {
    GameSceneLayoutHolder* getGameSceneLayoutHolder();
};
