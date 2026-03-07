#pragma once

#include "Game/LiveActor/FlashingCtrl.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"

class NoteFairy;

class NoteGroup : public LiveActorGroup {
public:
    NoteGroup();

    virtual ~NoteGroup();
    virtual void init(const JMapInfoIter&);
    virtual void movement();

    f32 mRotation;  // 0x18
};

/// @brief Class that represents a single note in a NoteFairy actor.
class Note : public LiveActor {
public:
    Note(const char*, const TVec3f&, NoteFairy*);

    virtual ~Note();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    /// @brief Checks if the sensor is either a player or a rideable entity and if it is, it will "collect" the note.
    virtual void attackSensor(HitSensor*, HitSensor*);

    void startCountDown();

    /// @brief Begins the countdown sequence that plays the "waiting" animation, and is switched to the countdown nerve when mIsCountdown is set to
    /// true by the Note::mParentFairy.
    void exeWait();
    /// @brief Begins the flashing sequence and if the current step reaches Note::mCounter's value, it calls Note::kill().
    void exeCountDown();
    /// @brief Scales the note's velocity by -30.0 and then invalidates all hit sensors, and after 10 steps, calls Note::kill().
    void exeFlyUp();

    /* 0x8C */ NoteFairy* mParentFairy;  ///< The parent NoteFairy that this note was spawned from.
    TVec3f _90;
    TVec3f mRailDirection;    // 0x9C
    /* 0xA8 */ int mCounter;  ///< The amount of steps it takes for the note to disappear.
    f32 _AC;
    bool mIsCountdown;                    // 0xB0
    /* 0xB4 */ FlashingCtrl* mFlashCtrl;  // A pointer to the FlashingCtrl instance that handles the "fading away" effects.
    u32 _B8;
};
