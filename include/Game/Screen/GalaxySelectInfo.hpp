#pragma once

#include "Game/Screen/LayoutActor.hpp"

class GalaxyInfoLayoutSetter;
class NameObjArchiveListCollector;
class ValueControl;

class GalaxySelectInfo : public LayoutActor {
public:
    GalaxySelectInfo(bool);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x18 */ virtual void draw() const;
    /* 0x24 */ virtual void appear();
    /* 0x28 */ virtual void kill();
    /* 0x2C */ virtual void control();

    bool show(const char*, u32, int);
    static void makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter);
    bool show(const char*, u32, bool, int);
    void exeWait();
    void exeDisappear() NO_INLINE;

    /* 0x20 */ bool _20;
    /* 0x24 */ const char* _24;
    /* 0x28 */ bool _28;
    /* 0x2C */ ValueControl* mFadeinoutControl;
    /* 0x30 */ GalaxyInfoLayoutSetter* mInfoLayoutSetter;
};
