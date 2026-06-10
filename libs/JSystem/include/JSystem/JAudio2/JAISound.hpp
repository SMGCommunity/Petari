#pragma once

#include "JSystem/JAudio2/JAIAudible.hpp"
#include "JSystem/JAudio2/JAISoundParams.hpp"
#include <stdint.h>

class JAISound;
class JASTrack;

class JAISoundID {
public:
    enum SoundType {
        SOUND_SE = 0,
        SOUND_SEQ = 1,
        SOUND_STREAM = 2,
    };

    /// @brief Creates a new `JAISoundID`.
    JAISoundID() {
    }

    JAISoundID(u32 id) {
        mID.mComposite = id;
    }

    JAISoundID(const JAISoundID& rOther) : mID(rOther.mID) {
    }

    JAISoundID(u32 sectionID, u32 groupID, u32 waveID) {
        mID.info.type.parts.sectionID = sectionID;
        mID.info.type.parts.groupID = groupID;
        mID.info.waveID = waveID;
    }

    operator u32() const {
        return mID.mComposite;
    }

    bool isAnonymous() const {
        return mID.mComposite == -1;
    }

    void setAnonymous() {
        mID.mComposite = -1;
    }

    u8 getSectionID() const {
        return mID.info.type.parts.sectionID;
    }

    void setSectionID(u8 id) {
        mID.info.type.parts.sectionID = id;
    }

    u8 getGroupID() const {
        return mID.info.type.parts.groupID;
    }

    void setGroupID(u8 id) {
        mID.info.type.parts.groupID = id;
    }

    u16 getWaveID() const {
        return mID.info.waveID;
    }

    void getWaveID(u16 id) {
        mID.info.waveID = id;
    }

    /* 0x00 */ union {
        u32 mComposite;
        struct {
            union {
                u16 value;
                struct {
                    u8 sectionID;
                    u8 groupID;
                } parts;
            } type;
            u16 waveID;
        } info;
    } mID;
};

class JAISoundHandle {
public:
    /// @brief Creates a new `JAISoundHandle`.
    JAISoundHandle() : mSound(nullptr) {
    }

    /// @brief Destroys the `JAISoundHandle`.
    ~JAISoundHandle() {
        releaseSound();
    }

    JAISound* operator->() const {
        return mSound;
    }

    JAISound* getSound() {
        return mSound;
    }

    bool isSoundAttached() const {
        return mSound != nullptr;
    }

    void releaseSound();

    /* 0x00 */ JAISound* mSound;
};

struct JAISoundStatus_ {
    s32 lockWhenPrepared();
    s32 unlockIfLocked();

    enum State {
        State_PREPARE = 0,
        State_LOCK_PREPARE = 1,
        State_STOP = 2,
        State_LOCK_READY = 3,
        State_READY = 4,
        State_PLAYING = 5,
        State_DEAD = 6,
    };

    void init() {
        _0.value = 0;
        _1.value = 0;
        *((u16*)&mState) = 0;
        mUserData = 0;
    }

    u8 getState() const {
        return mState.state;
    }

    u8 setState(u8 state) {
        return mState.state = state;
    }

    bool isAlive() const {
        return getState() != State_DEAD;
    }
    bool isDead() const {
        return getState() == State_DEAD;
    }
    void die() {
        setState(State_DEAD);
    }
    void stop() {
        setState(State_STOP);
    }
    bool isStopped() const {
        return getState() == State_STOP;
    }

    bool isPlaying() const {
        return getState() == State_PLAYING;
    }
    void setPlaying() {
        setState(State_PLAYING);
    }

    void setReadyLocked() {
        setState(State_LOCK_READY);
    }

    void setReady() {
        setState(State_READY);
    }

    bool isPrepared() const {
        return getState() >= State_LOCK_READY;
    }
    bool isMute() const {
        return _0.flags.mute;
    }

    void pause(bool pause) {
        _0.flags.paused = pause;
    }
    bool isPaused() const {
        return _0.flags.paused;
    }

    void pauseWhenOut() {
        _1.flags.pauseWhenOut = true;
    }

    bool comesBack() const {
        return _1.flags.comesBack;
    }
    bool hasLifeTime() const {
        return _1.flags.hasLifeTime;
    }
    bool isPauseWhenOut() const {
        return _1.flags.pauseWhenOut;
    }

    bool isCalcedOnce() const {
        return mState.flags.calcedOnce;
    }
    void setCalcedOnce(bool calcedOnce) {
        mState.flags.calcedOnce = calcedOnce;
    }

    // TODO: states context is in JAUSoundAnimator::ageSounds_
    s32 getAnimationState() const {
        return mState.flags.animationState;
    }

    void setAnimationState(u32 state) {
        mState.flags.animationState = state;
    }

    uintptr_t getUserData() const {
        return mUserData;
    }

    /* 0x0 */ union {  // TODO: is this just JAISoundActivity?
        u8 value;
        struct {
            u8 mute : 1;
            u8 paused : 1;
            u8 flag3 : 1;
            u8 flag4 : 1;
            u8 flag5 : 1;
            u8 flag6 : 1;
            u8 flag7 : 1;
            u8 flag8 : 1;
        } flags;
    } _0;
    /* 0x1 */ union {
        u8 value;
        struct {
            u8 comesBack : 1;
            u8 hasLifeTime : 1;
            u8 pauseWhenOut : 1;
            u8 flag4 : 1;
            u8 flag5 : 1;
            u8 flag6 : 1;
            u8 flag7 : 1;
            u8 flag8 : 1;
        } flags;
    } _1;
    /* 0x2 */ struct {
        u8 state;
        struct {
            u8 flag1 : 1;
            u8 calcedOnce : 1;
            u8 animationState : 2;
            u8 flag5 : 1;
            u8 flag6 : 1;
            u8 flag7 : 1;
            u8 flag8 : 1;
        } flags;
    } mState;
    /* 0x4 */ uintptr_t mUserData;
};

struct JAISoundFader {
    void forceIn() {
        mValue = 1.0f;
        mTransition.zero();
    }

    void forceOut() {
        mValue = 0.0f;
        mTransition.zero();
    }

    void fadeOut(u32 maxSteps) {
        if (maxSteps != 0) {
            mTransition.set(0.0f, mValue, maxSteps);
        } else {
            forceOut();
        }
    }

    void fadeIn(u32 maxSteps) {
        if (maxSteps != 0) {
            mTransition.set(1.0f, mValue, maxSteps);
        } else {
            forceIn();
        }
    }

    void fadeInFromOut(u32 maxSteps) {
        mValue = 0.0f;
        fadeIn(maxSteps);
    }

    bool isOut() const {
        return (mTransition.mRemainingSteps == 0 && mValue < 0.01f);
    }

    void calc() {
        mValue = mTransition.apply(mValue);
    }
    f32 getIntensity() const {
        return mValue;
    }

    /* 0x00 */ f32 mValue;
    /* 0x04 */ JAISoundParamsTransition::TTransition mTransition;
};

template < typename T >
struct JAISoundStrategy {
    virtual ~JAISoundStrategy();
    virtual void calc(T*);
    virtual void mix(T*, const JASSoundParams&);
};

template < typename T >
struct JAISoundStrategyMgr {
    virtual ~JAISoundStrategyMgr();
    virtual JAISoundStrategy< T >* calc(JAISoundID);
    virtual void deleteStrategy(JAISoundStrategy< T >*);
};

class JAISoundActivity {
public:
    void init() {
        _0.value = 0;
    }

    void mute(bool mute) {
        _0.flags.muted = mute;
    }

    bool isMute() const {
        return _0.flags.muted;
    }

    void pause(bool pause) {
        _0.flags.paused = pause;
    }

    bool isPaused() const {
        return _0.flags.paused;
    }

    /* 0x0 */ union {
        u8 value;
        struct {
            u8 muted : 1;
            u8 paused : 1;
            u8 flag3 : 1;
            u8 flag4 : 1;
            u8 flag5 : 1;
            u8 flag6 : 1;
            u8 flag7 : 1;
            u8 flag8 : 1;
        } flags;
    } _0;
};

class JAISoundActivityMgr {
public:
    JAISoundActivity mSoundActivity;
};

class JAITempoMgr {
public:
    JAITempoMgr() {
        init();
    }
    void init() {
        setTempo(1.0f);
    }
    void setTempo(f32 tempo) {
        mTempo = tempo;
        mTransition.zero();
    }
    f32 getTempo() const {
        return mTempo;
    }
    void calc() {
        mTempo = mTransition.apply(mTempo);
    }

    /* 0x00 */ f32 mTempo;
    /* 0x04 */ JAISoundParamsTransition::TTransition mTransition;
};

class JAISoundHandle;
class JAIAudible;
struct JAIAudience;
class JAISe;
class JAISeq;
struct JAISoundChild;
class JAIStream;
class JAITempoMgr;

class JAISound {
public:
    // TODO: a lot of these flag reads and writes can be forwarded by status getters and setters

    /// @brief Creates a new `JAISound`.
    JAISound();

    void releaseHandle();
    void attachHandle(JAISoundHandle* handle);

    void start_JAISound_(JAISoundID id, const TVec3f* posPtr, JAIAudience* audience);
    bool acceptsNewAudible() const;
    void newAudible(const TVec3f&, TVec3f const*, u32, JAIAudience*);
    void stop();
    void stop(u32 fadeTime);
    void die_JAISound_();
    void increasePrepareCount_JAISound_();
    bool calc_JAISound_();
    void initTrack_JAISound_(JASTrack* track);

    virtual s32 getNumChild() const = 0;
    virtual JAISoundChild* getChild(int) = 0;
    virtual void releaseChild(int) = 0;
    virtual JAISe* asSe();
    virtual JAISeq* asSeq();
    virtual JAIStream* asStream();
    virtual JASTrack* getTrack() = 0;
    virtual JASTrack* getChildTrack(int) = 0;
    virtual JAITempoMgr* getTempoMgr() = 0;
    virtual bool JAISound_tryDie_() = 0;

    JAISoundID getID() const {
        return mSoundID;
    }
    u32 getAnimationState() const {
        return mStatus.getAnimationState();
    }
    bool isAnimated() const {
        return getAnimationState() != 0;
    }
    void setAnimationState(u32 state) {
        mStatus.setAnimationState(state);
    }
    uintptr_t getUserData() const {
        return mStatus.getUserData();
    }
    void setUserData(uintptr_t userData) {
        mStatus.mUserData = userData;
    }
    JAIAudible* getAudible() {
        return mAudible;
    }
    bool isHandleAttached() const {
        return mHandle != nullptr;
    }

    bool isAlive() const {
        return mStatus.isAlive();
    }
    bool isDead() const {
        return mStatus.isDead();
    }
    void die() {
        mStatus.die();
    }

    bool isPlaying() const {
        return mStatus.isPlaying();
    }
    bool isMute() const {
        return mStatus.isMute();
    }

    bool isPauseWhenOut() const {
        return mStatus.isPauseWhenOut();
    }

    bool comesBack() const {
        return mStatus.comesBack();
    }
    bool hasLifeTime() const {
        return mStatus.hasLifeTime();
    }

    void setCalcedOnce(bool calcedOnce) {
        mStatus.setCalcedOnce(calcedOnce);
    }
    bool isCalcedOnce() const {
        return mStatus.isCalcedOnce();
    }

    void removeLifeTime_() {
        mStatus._1.flags.comesBack = false;
        mStatus._1.flags.hasLifeTime = false;
    }

    bool isPrepared() const {
        return mStatus.isPrepared();
    }
    void unlockIfLocked() {
        mStatus.unlockIfLocked();
    }
    void lockWhenPrepared() {
        mStatus.lockWhenPrepared();
    }
    void stop_JAISound_() {
        mStatus.mState.flags.flag5 = 0;
        mStatus.mState.flags.flag1 = 1;
    }

    bool isStopping() const {
        return mStatus.mState.flags.flag1 && (!mStatus.mState.flags.flag5 || mFader.isOut());
    }

    void pause(bool pause) {
        mStatus.pause(pause);
    }

    bool isPaused() const {
        return mStatus.isPaused();
    }

    void pauseWhenOut() {
        mStatus.pauseWhenOut();
    }

    void updateLifeTime(u32 lifeTime) {
        if (lifeTime > mLifeTime) {
            mLifeTime = lifeTime;
        }
    }

    void setLifeTime(u32 lifeTime, bool pauseWhenOut) {
        mLifeTime = lifeTime;
        setComesBack(pauseWhenOut);
        mStatus._1.flags.hasLifeTime = true;
    }

    void setComesBack(bool pauseWhenOut) {
        mStatus._1.flags.comesBack = true;
        if (pauseWhenOut) {
            mStatus.pauseWhenOut();
        }
    }

    bool setPos(const TVec3f& pos) {
        if (mAudible != nullptr) {
            mAudible->setPos(pos);
        }
        return mAudible != nullptr;
    }

    JAISoundFader* getFader() const {
        return (JAISoundFader*)&mFader;
    }
    void fadeIn(u32 maxSteps) {
        mFader.fadeInFromOut(maxSteps);
    }
    void fadeOut(u32 maxSteps) {
        mFader.fadeOut(maxSteps);
    }

    JAISoundParamsProperty& getProperty() {
        return mParams.mProperty;
    }

    s32 getCount() const {
        return mCount;
    }

    JAISoundParamsMove& getAuxiliary() {
        return mParams.mMove;
    }

    /* 0x04 */ JAISoundHandle* mHandle;
    /* 0x08 */ JAIAudible* mAudible;
    /* 0x0C */ JAIAudience* mAudience;
    /* 0x10 */ u32 mLifeTime;
    /* 0x14 */ s32 mPrepareCount;
    /* 0x18 */ JAISoundID mSoundID;
    /* 0x1C */ JAISoundStatus_ mStatus;
    /* 0x24 */ JAISoundFader mFader;
    /* 0x34 */ u32 mPriority;
    /* 0x38 */ s32 mCount;
    /* 0x3C */ JAISoundParams mParams;
};
