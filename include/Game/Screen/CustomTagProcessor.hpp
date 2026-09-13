#pragma once

#include "Game/Screen/MessageTagSkipTagProcessor.hpp"
#include <revolution/gx/GXStruct.h>

namespace nw4r {
    namespace lyt {
        class TextBox;
    }
}  // namespace nw4r

class CustomTagAlphaCtrl {
public:
    CustomTagAlphaCtrl();

    void init(u32, f32, f32, s32, s32);
    u8 alpha() const;
    void update();
    bool isEnd() const;

    /* 0x00 */ s32 mStartDelay;
    /* 0x04 */ s32 mEndDelay;
    /* 0x08 */ s32 mFrame;
    /* 0x0C */ s32 mCharIndex;
    /* 0x10 */ s32 mWaitTime;
    /* 0x14 */ u32 mCharCount;
    /* 0x18 */ bool mEnabled;
    /* 0x1C */ f32 mCharInterval;
    /* 0x20 */ f32 mFadeSpeed;
};

class CustomTagProcessor : public MessageTagSkipTagProcessor {
public:
    CustomTagProcessor(nw4r::lyt::TextBox*);

    virtual ~CustomTagProcessor();
    virtual Operation Process(u16, ContextType*) override;
    virtual Operation CalcRect(nw4r::ut::Rect*, u16, ContextType*) override;

    void setArgNumber(s32, s32);
    void setArgString(const wchar_t*, s32);
    void initAlpha(f32, f32, s32, s32);
    void reset(const wchar_t*);
    MessageEditorMessageTag getReplaceTag(const wchar_t*, s32, s32, s32) const;
    bool isIgnoreTag(u16, ContextType*) const;
    bool writeString(nw4r::ut::Rect*, const wchar_t*, ContextType*);
    Operation exeDisplayGroup(nw4r::ut::Rect*, const MessageEditorMessageTag&, ContextType*);
    Operation exeSoundGroup(nw4r::ut::Rect*, const MessageEditorMessageTag&, ContextType*);
    Operation exePictureGroup(nw4r::ut::Rect*, const MessageEditorMessageTag&, ContextType*);
    Operation exeFontSizeGroup(nw4r::ut::Rect*, const MessageEditorMessageTag&, ContextType*);
    Operation exeSystemGroup(nw4r::ut::Rect*, const MessageEditorMessageTag&, ContextType*);
    Operation exeLocalizeGroup(nw4r::ut::Rect*, const MessageEditorMessageTag&, ContextType*);
    Operation exeNumberGroup(nw4r::ut::Rect*, const MessageEditorMessageTag&, ContextType*);
    Operation exeStringGroup(nw4r::ut::Rect*, const MessageEditorMessageTag&, ContextType*);
    Operation exeSystemGroupColor(nw4r::ut::Rect*, int, ContextType*);
    Operation exeSystemGroupRuby(nw4r::ut::Rect*, const MessageEditorMessageTag&, ContextType*);
    Operation exeDisplayGroupWait(nw4r::ut::Rect*, u16, ContextType*) NO_INLINE;
    Operation exeDisplayGroupOffset(nw4r::ut::Rect*, const MessageEditorMessageTag&, ContextType*);
    Operation exeDisplayGroupCenter(nw4r::ut::Rect*, const MessageEditorMessageTag&, ContextType*);
    Operation exeFontGroup(nw4r::ut::Rect*, const MessageEditorMessageTag&, ContextType*);
    Operation exePatchimuGroup(nw4r::ut::Rect*, const MessageEditorMessageTag&, ContextType*);

    struct Impl {
        struct GroupFunctionInfo {
            u8 mGroup;
            Operation (CustomTagProcessor::*mFunction)(nw4r::ut::Rect*, const MessageEditorMessageTag&, ContextType*);
        };

        static GroupFunctionInfo* findGroupFunctionInfo(int);
        static GroupFunctionInfo sGroupFunctionTable[];
    };

    /* 0x04 */ bool mIsShadow;
    /* 0x05 */ bool mIsText;
    /* 0x06 */ bool mIsInfo;
    /* 0x07 */ bool mNoCharSpace;
    /* 0x08 */ CustomTagAlphaCtrl mAlphaCtrl;
    /* 0x2C */ nw4r::lyt::TextBox* mTextBox;
    /* 0x30 */ u8 mPlayedSounds;
    /* 0x31 */ u8 mSoundIndex;
    /* 0x32 */ u8 mColorIndex;
    /* 0x34 */ u16 mPreviousChar;
    /* 0x36 */ GXColor mColorMappingMin;
    /* 0x3A */ GXColor mColorMappingMax;
    /* 0x40 */ f32 mRubyFontWidth;
    /* 0x44 */ f32 mRubyFontHeight;
    /* 0x48 */ f32 mFontWidth;
    /* 0x4C */ f32 mFontHeight;
};
