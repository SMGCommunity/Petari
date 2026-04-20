#pragma once

#include "Game/Scene/MultiSceneActor.hpp"

class ScenarioSelectStar : public MultiSceneActor {
public:
    ScenarioSelectStar(EffectSystem*);

    virtual ~ScenarioSelectStar();
    virtual void init(const JMapInfoIter&);
    virtual void calcViewAndEntry();
    virtual void appear();
    virtual void kill();
    virtual void control();

    void tryPointing();
    void select();
    void notSelect();
    bool isAppearEnd() const;
    void setup(s32, int, const TVec3f&, s32);
    bool tryEndPointing();
    void updatePos();
    void exeAppear();
    void exeNotPointing();
    void exePointing();
    void exeEndPointing();
    void exeSelected();
    void exeSelectedMove();
    void exeNotSelected();

    /* 0x44 */ int mStarCollectedStatus;
    /* 0x48 */ s32 mStageID;
    /* 0x4C */ s32 mStarIdx;
    /* 0x50 */ bool mIsPointing;
    /* 0x54 */ f32 mRotateSpeed;
    /* 0x58 */ s32 mRotateTime;
    /* 0x5C */ TVec3f mBasePos;
    /* 0x68 */ TVec3f mTranslationOnSelect;
    /* 0x74 */ int mAppearFrame;
    /* 0x78 */ f32 mScaleOnSelect;
};
