#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;
class RunawayRabbit;
class RunawayTico;

class RunawayRabbitCollect : public LiveActor {
public:
    /// @brief Creates a new `RunawayRabbitCollect`.
    /// @param pName A pointer to the null-terminated name of the object.
    RunawayRabbitCollect(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void control();

    s32 calcCompleteRabbitCount() const;
    void linkMsgCtrl();
    void noticeAppearRabbit(RunawayRabbit*);
    void noticeCaughtRabbit(RunawayRabbit*);
    void appearTico(RunawayTico*, const TVec3f&);

    void exeWait();
    void exeActive();

private:
    /* 0x8C */ ActorCameraInfo* mCameraInfo;
    /* 0x90 */ RunawayRabbit** mRabbit;
    /* 0x94 */ s32 mRabbitNum;
    /* 0x98 */ RunawayTico** mTico;
    /* 0x9C */ s32 mTicoNum;
    /* 0xA0 */ u32 _A0;
    /* 0xA4 */ u32 _A4;
    /* 0xA8 */ s32 mCompleteRabbitCount;
    /* 0xAC */ s32 mBgmState;
    /* 0xB0 */ bool _B0[4];
};
