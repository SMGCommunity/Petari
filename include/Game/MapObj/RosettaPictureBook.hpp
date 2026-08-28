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

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    static void makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter);

    void exeWait();
    void exeDemoWait();
    void exeFadeOut();
    void exeReading();
    void exeFadeIn();

private:
    /* 0x8C */ PictureBookLayout* mLayout;
    /* 0x90 */ IconAButton* mIconAButton;
    /* 0x94 */ bool mIsValidOpenIconAButton;
};
