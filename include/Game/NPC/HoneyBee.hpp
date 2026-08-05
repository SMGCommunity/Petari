#pragma once

#include "Game/NPC/NPCActor.hpp"

class HoneyBee : public NPCActor {
public:
    enum InitAction {
        /* 0x00 */ InitAction_Wait,
        /* 0x01 */ InitAction_JumpLecture,
        /* 0x02 */ InitAction_Fly,
        /* 0x03 */ InitAction_Sleep,
        /* 0x04 */ InitAction_Stick,
        /* 0x05 */ InitAction_Gatekeeper,
        /* 0x06 */ InitAction_DropLecture,
        /* 0x07 */ InitAction_Flustered,
        /* 0x08 */ InitAction_FlyLectureA,
        /* 0x09 */ InitAction_FlyLectureB,
    };

    /// @brief Creates a new `HoneyBee`.
    /// @param pName A pointer to the null-terminated name of the object.
    HoneyBee(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    void exeWait();
    void exeReaction();
    void exeFly();
    void exeDropLecture();
    void exeJumpLecture();
    void exeFlyLectureA();
    void exeFlyLectureB();

    /* 0x15C */ s32 _15C;
    /* 0x160 */ TMtx34f _160;
    /* 0x190 */ TVec3f _190;
    /* 0x19C */ s32 mArg0;
    /* 0x1A0 */ f32 _1A0;
};
