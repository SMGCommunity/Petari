#pragma once

#include "Game/NPC/Kinopio.hpp"

class LayoutActor;
class LuigiLetter;
class PeachLetter;
class ReceiverTagMail;

class KinopioAstro : public Kinopio {
public:
    enum LetterEvent {
        /* 0x0 */ LetterEvent_LuigiLetter,
        /* 0x1 */ LetterEvent_PeachLetterNormal,
        /* 0x2 */ LetterEvent_PeachLetter1Ups,
        /* 0x3 */ LetterEvent_3,
        /* 0x4 */ LetterEvent_SendLetter,
        /* 0x5 */ LetterEvent_OnMsgLedPattern,
        /* 0x6 */ LetterEvent_OffMsgLedPattern,
    };

    /// @brief Creates a new `KinopioAstro`.
    /// @param pName A pointer to the null-terminated name of the object.
    KinopioAstro(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    bool dispLetter(LayoutActor*) const;
    bool isDispLetterIcon() const;
    void createLetterIcon(const JMapInfoIter&);
    bool sendLetter();
    bool branchFunc(u32);
    bool eventFunc(u32);
    void startDemo();
    void endDemo();

    /* 0x188 */ LuigiLetter* mLuigiLetter;
    /* 0x18C */ PeachLetter* mPeachLetter;
    /* 0x190 */ TalkMessageCtrl* mLetterIconTalkCtrl;
    /* 0x194 */ bool _194;
    /* 0x195 */ bool _195;
    /* 0x196 */ bool mIsPlayerLuigi;
    /* 0x197 */ bool _197;
    /* 0x198 */ ReceiverTagMail* _198;
    /* 0x19C */ ResourceHolder* _19C;
};
