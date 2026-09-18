#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class IconAButton;
class NameObjArchiveListCollector;
class PictureBookLayout;

class RosettaPictureBook : public LiveActor {
public:
    /// @brief Creates a new `RosettaPictureBook`.
    /// @param pName A pointer to the null-terminated name of the object.
    RosettaPictureBook(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    static void makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter);

    void exeWait();
    void exeDemoWait();
    void exeFadeOut();
    void exeReading();
    void exeFadeIn();

public:
    /* 0x8C */ PictureBookLayout* mLayout;
    /* 0x90 */ IconAButton* mIconAButton;
    /* 0x94 */ bool mIsValidOpenIconAButton;
};
