#pragma once

#include "Game/Animation/XanimeCore.hpp"
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class J3DAnmTransform;
class J3DModel;
class XanimeGroupInfo;
class XanimeResourceTable;

class XanimeFrameCtrl : public J3DFrameCtrl {
public:
    /// @brief Creates a new `XanimeFrameCtrl`.
    XanimeFrameCtrl();

    /* 0x14 */ u32 _14;
};

class XanimePlayer {
public:
    XanimePlayer(J3DModel*, XanimeResourceTable*);
    XanimePlayer(J3DModel*, XanimeResourceTable*, XanimePlayer*);

    void init();
    void stopAnimation();
    void stopAnimation(const char*);
    void changeTrackAnimation(u8, const char*);
    void changeTrackAnimation(u8, u32);
    void changeAnimationBck(const char*);
    bool changeSimpleBck(const char*);
    void changeAnimation(const char*);
    void changeAnimationByHash(u32);
    void swapFrameCtrl(const XanimeGroupInfo*);
    void changeAnimation(const XanimeGroupInfo*);
    void prepareAnimation(const XanimeGroupInfo*);
    void runNextAnimation();
    void changeAnimationSimple(J3DAnmTransform*);
    void changeSpeed(f32);
    void changeInterpoleFrame(s32);
    bool changeTrackWeight(u32, f32);
    void calcAnm(u16);
    void setModel(J3DModel*);
    void overWriteMtxCalc(u16);
    void clearMtxCalc(u16);
    void clearAnm(u16);
    void updateBeforeMovement();
    void updateAfterMovement();
    void updateInterpoleRatio();
    void getMainAnimationTrans(u32, TVec3f*) const;
    bool isRun(const char*) const;
    bool isTerminate(const char*) const;
    bool isTerminate() const;
    void setDefaultAnimation(const char*);
    void runDefaultAnimation();
    bool isAnimationRunSimple() const;
    const char* getCurrentAnimationName() const;
    const char* getDefaultAnimationName() const;
    const char* getCurrentBckName() const;
    const char* getNameStringPointer(const char*) const;
    bool checkPass(f32) const;
    f32 tellAnimationFrame() const;
    const XanimeGroupInfo* getSimpleGroup() const;
    void duplicateSimpleGroup();
    void changeCurrentAnimation(const XanimeGroupInfo*);

    inline XanimeCore* getCore() { return mCore; }

    /* 0x00 */ J3DModel* mModel;
    /* 0x04 */ J3DModelData* mModelData;
    /* 0x08 */ f32 _08;
    /* 0x0C */ f32 _0C;
    /* 0x10 */ f32 _10[4];
    /* 0x20 */ XanimeFrameCtrl* _20;
    /* 0x24 */ XanimeFrameCtrl _24[2];
    /* 0x54 */ u8 _54;
    /* 0x55 */ u8 _55;
    /* 0x58 */ const char* mCurrentBckName;
    /* 0x5C */ const XanimeGroupInfo* mDefaultAnimation;
    /* 0x60 */ const XanimeGroupInfo* mCurrentAnimation;
    /* 0x64 */ const XanimeGroupInfo* mPrevAnimation;
    /* 0x68 */ const XanimeGroupInfo* _68;
    /* 0x6C */ XanimeCore* mCore;
    /* 0x70 */ XanimeResourceTable* mResourceTable;
    /* 0x74 */ XanimeGroupInfo* _74;
    /* 0x78 */ s32 _78;
    /* 0x7C */ u8 _7C;
    /* 0x7D */ u8 _7D;
    /* 0x7E */ u8 _7E;
    /* 0x7F */ u8 _7F;
    /* 0x80 */ u8 _80;
    /* 0x84 */ f32 _84;
    /* 0x88 */ u8 _88;
};
