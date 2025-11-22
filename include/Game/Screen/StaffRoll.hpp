#pragma once

#include "Game/Screen/LayoutActor.hpp"

namespace nw4r {
    namespace lyt {
        class TexMap;
    };
}; // namespace nw4r

class InformationMessage;

class StaffRollLine : public LayoutActor {
public:
    /// @brief Creates a new `StaffRollLine`.
    /// @param pName A pointer to the null-terminated name of the object.
    StaffRollLine(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();

    void exeScroll();

private:
    /* 0x20 */ TVec2f mPosition;
};

class StaffRollPicture : public LayoutActor {
public:
    /// @brief Creates a new `StaffRollPicture`.
    /// @param pName A pointer to the null-terminated name of the object.
    StaffRollPicture(const char* pName);

    virtual void init(const JMapInfoIter& rIter);

    void initReplaceTexture();
    void startPictureAction(s32, const char*, const char*);
    void disappearAllPane();
    void exeWork();

private:
    /* 0x20 */ nw4r::lyt::TexMap** mReplaceTexture;
};

class StaffRoll : public LayoutActor {
public:
    /// @brief Creates a new `StaffRoll`.
    /// @param pName A pointer to the null-terminated name of the object.
    StaffRoll(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void kill();

    void           startInfo();
    bool           isPauseOrEnd() const;
    bool           isNextLineEmpty() const;
    void           setNrvNextInfo();
    bool           tryGenerateLine();
    void           makeLine();
    StaffRollLine* getDeadLineLayout() const;
    void           exePrepareDemo();
    void           exePrepareBgm();
    void           exeTitleLogoAppear();
    void           exeTitleLogoScroll();
    void           exeLineScroll();
    void           exeEndLogoScroll();
    void           exeAllRights();
    void           exePrepareInfo();
    void           exeInfoAppear();
    void           exeInfoDisappear();
    void           exePause();
    void           exeRestart();
    void           exePrepareSave();
    void           exeSave();
    void           exeEnd();

private:
    /* 0x20 */ InformationMessage* mInfo;
    /* 0x24 */ StaffRollLine**     mLine;
    /* 0x28 */ StaffRollPicture*   mPicture;
    /* 0x2C */ const wchar_t*      mMsg;
    /* 0x30 */ u32                 mMsgLineNum;
    /* 0x34 */ u32                 mMsgLine;
    /* 0x38 */ TVec2f              mTitleLogoPos;
    /* 0x40 */ TVec2f              mEndLogoPos;
    /* 0x48 */ s32                 _48;
    /* 0x4C */ s32                 _4C;
};

namespace MR {
    StaffRoll* getStaffRoll();
};
