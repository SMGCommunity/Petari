#pragma once

#include "Game/NPC/Kinopio.hpp"

class LayoutActor;
class LuigiLetter;
class PeachLetter;
class ReceiverTagMail;

class KinopioAstro : public Kinopio {
public:
    /// @brief Creates a new `KinopioAstro`.
    /// @param pName A pointer to the null-terminated name of the object.
    KinopioAstro(const char* pName);

    virtual ~KinopioAstro();
    virtual void init(const JMapInfoIter& rIter);
    virtual void control();

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    bool dispLetter(LayoutActor*) const;
    bool isDispLetterIcon() const;
    void createLetterIcon(const JMapInfoIter&);
    bool sendLetter();
    bool branchFunc(u32);
    void eventFunc(u32);
    void startDemo();
    void endDemo();

private:
    /* 0x188 */ LuigiLetter* LuigiLetter;
    /* 0x18C */ PeachLetter* mPeachLetter;
    /* 0x190 */ TalkMessageCtrl* mLetterIconTalkCtrl;
    /* 0x194 */ bool _194;
    /* 0x195 */ bool _195;
    /* 0x196 */ bool _196;
    /* 0x197 */ bool _197;
    /* 0x198 */ ReceiverTagMail* _198;
    /* 0x19C */ ResourceHolder* _19C;
};
