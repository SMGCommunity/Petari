#pragma once

#include "Game/Screen/LayoutActor.hpp"

namespace nw4r {
    namespace lyt {
        class TexMap;
    };
};

class IconAButton;
class NameObjArchiveListCollector;

class LuigiLetter : public LayoutActor {
public:
    /// @brief Creates a new `LuigiLetter`.
    LuigiLetter(bool, const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void movement();

    void exeAppear();
    void exeWait();
    void exeEnd();
    static void makeArchiveListForNPC(NameObjArchiveListCollector*, const JMapInfoIter&);
    static void makeArchiveListForMenu(NameObjArchiveListCollector*, const JMapInfoIter&);
    static void makeArchiveListCommon(NameObjArchiveListCollector*, const char*);

private:
    /* 0x20 */ nw4r::lyt::TexMap* mTexture;
    /* 0x24 */ IconAButton* mAButtonIcon;
    /* 0x28 */ const char* _28;
    /* 0x2C */ bool _2C;
};

namespace MR {
    LuigiLetter* createLuigiLetterForTalk();
    LuigiLetter* createLuigiLetterForMenu();
};
