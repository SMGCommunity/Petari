#pragma once

#include "Game/Screen/LayoutActor.hpp"

namespace nw4r {
    namespace lyt {
        class TexMap;
    };
};

class ButtonPaneController;
class IconAButton;
class NameObjArchiveListCollector;
class PictureBookCloseButton;

class PictureBookLayout : public LayoutActor {
public:
    PictureBookLayout(s32 chapterMin, s32 chapterMax, bool isRosettaReading);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();

    static void makeArchiveList(NameObjArchiveListCollector* pCollector, s32 chapterMin, s32 chapterMax, bool isRosettaReading);

    s32 getChapterMax();

    void initTexture();
    void initContentsButton();
    bool updateText();
    void updateTexture();
    bool textNext();
    bool pageNext();
    bool chapterNext();
    void updateTexMapChapterBase();
    bool isReadedCurrentText() const;
    s32 getReadSpeed() const;
    bool isBookEndCurrentText() const;
    void setTextAlpha(f32 alpha);
    s32 getChapterNumberMax() const;
    bool isValidCloseButton() const;
    bool isSelectedCloseButton() const;
    s32 getCurrentMaxTextIndex() const;
    void exeOpen();
    void exeContentsSelect();
    void exeContentsFadeOut();
    void exeFadeIn();
    void exeWaitNoText();
    void exeFadeInText();
    void exeWaitWithText();
    void exeFadeOutText();
    void exePageNext();
    void exeFadeOut();
    void exeClose();
    void hideContents();
    f32 getFadeInAlphaTextBG(f32 alpha) const;
    f32 getFadeOutAlphaTextBG(f32 alpha) const;

private:
    /* 0x20 */ s32 mChapterMin;
    /* 0x24 */ s32 mChapterMax;
    /* 0x28 */ s32 mChapterRosettaMax;
    /* 0x2C */ s32 mChapterNo;
    /* 0x30 */ s32 mPageNo;
    /* 0x34 */ s32 mTextIndex;
    /* 0x38 */ s32 mNotReadedChapterNo;
    /* 0x3C */ s32 mNotReadedPageNo;
    /* 0x40 */ s32 mNotReadedTextIndex;
    /* 0x44 */ nw4r::lyt::TexMap** _44;
    /* 0x48 */ nw4r::lyt::TexMap** _48;
    /* 0x4C */ nw4r::lyt::TexMap* mTitleTexMap;
    /* 0x50 */ nw4r::lyt::TexMap* mCoverFrontTexMap;
    /* 0x54 */ nw4r::lyt::TexMap* mCoverBackTexMap;
    /* 0x58 */ s32 mNextItemDir;
    /* 0x5C */ bool mIsNextItemFast;
    /* 0x60 */ IconAButton* mIconAButton;
    /* 0x64 */ ButtonPaneController** mContentsButtonPaneController;
    /* 0x68 */ PictureBookCloseButton* mCloseButton;
};
