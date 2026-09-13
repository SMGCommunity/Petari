#include "Game/Screen/CustomTagProcessor.hpp"
#include "Game/Screen/ReplaceTagProcessor.hpp"
#include "Game/System/Language.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include <algorithm>
#include <cstdio>
#include <nw4r/lyt/material.h>
#include <nw4r/lyt/textBox.h>
#include <nw4r/ut/TextWriterBase.h>

extern "C" int swprintf(wchar_t*, size_t, const wchar_t*, ...);

void CustomTagProcessor_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)2.0f;
    (void)255.0f;
    (void)nw4r::math::F_MAX;
    (void)-2.0f;
    (void)0.75f;
    (void)1.5f;
    (void)0.175f;
}

namespace {
    const f32 mRubyBaseLineOffset = 3.0f;
    const f32 mRubyCharSpace = 2.0f;
    const f32 mFontSizeOffset = 0.175f;
    const f32 mPictureFontOffset = -2.0f;
    const f32 mOffsetOffset = 0.0f;

    GXColor sTextColor[] = {{255, 255, 255, 255}, {220, 130, 130, 255}, {80, 170, 80, 255},   {80, 140, 210, 255},
                            {235, 200, 0, 255},   {180, 110, 200, 255}, {255, 190, 190, 255}, {110, 243, 70, 255},
                            {120, 255, 255, 255}, {255, 255, 80, 255},  {251, 188, 250, 255}, {190, 190, 200, 255}};
    GXColor sTextColorKorean[] = {{255, 255, 255, 255}, {230, 160, 0, 255},   {80, 170, 80, 255},   {80, 140, 210, 255},
                                  {235, 200, 0, 255},   {180, 110, 200, 255}, {255, 210, 80, 255},  {110, 243, 70, 255},
                                  {120, 255, 255, 255}, {255, 255, 80, 255},  {251, 188, 250, 255}, {190, 190, 200, 255}};

    u8 clampU8(s32 value) NO_INLINE;

    GXColor setGXColor(GXColorS10 color) {
        GXColor result;
        result.r = clampU8(color.r);
        result.g = clampU8(color.g);
        result.b = clampU8(color.b);
        result.a = clampU8(color.a);
        return result;
    }

    u8 clampU8(s32 value) {
        return value < 0 ? 0 : value > 255 ? 255 : value;
    }

    const GXColor& getTextColor(s32 index) NO_INLINE;

    const GXColor& getTextColor(s32 index) {
        return MR::getLanguage() == 0x49 ? sTextColorKorean[index] : sTextColor[index];
    }

    void setTextColor(nw4r::ut::TextWriterBase< wchar_t >* writer, s32 index) {
        GXColor color = getTextColor(index);
        color.a = writer->GetTextColor().a;
        writer->SetTextColor(color);
    }
}  // namespace

CustomTagAlphaCtrl::CustomTagAlphaCtrl() : mStartDelay(), mEndDelay(), mFrame() {
    mCharInterval = 0.0f;
    mFadeSpeed = 0.0f;
    mCharIndex = 0;
    mCharCount = 0;
    mWaitTime = 0;
    mEnabled = false;
}

void CustomTagAlphaCtrl::init(u32 count, f32 speed, f32 interval, s32 startDelay, s32 endDelay) {
    if (0.0f == speed) {
        mEnabled = false;
        return;
    }

    mEnabled = true;
    mFrame = -startDelay;
    mCharIndex = 0;
    mCharInterval = interval;
    mFadeSpeed = speed;
    mCharCount = count;
    mWaitTime = 0;
    mStartDelay = startDelay;
    mEndDelay = endDelay;
}

u8 CustomTagAlphaCtrl::alpha() const {
    if (mEnabled) {
        f32 value = mFadeSpeed * (mFrame - mWaitTime) - mCharIndex * mCharInterval;
        return 255.0f * std::max(0.0f, std::min(1.0f, value));
    }

    return 255;
}

void CustomTagAlphaCtrl::update() {
    s32 delay = mWaitTime + mEndDelay;
    mFrame = std::min(mFrame + 1, static_cast< s32 >((1.0f + mCharCount * mCharInterval) / mFadeSpeed) + delay);
}

bool CustomTagAlphaCtrl::isEnd() const {
    if (mEnabled) {
        s32 frame = mFrame;
        s32 delay = mWaitTime + mEndDelay;
        return frame >= static_cast< s32 >((1.0f + mCharCount * mCharInterval) / mFadeSpeed) + delay;
    }

    return true;
}

CustomTagProcessor::Impl::GroupFunctionInfo CustomTagProcessor::Impl::sGroupFunctionTable[] = {
    {1, &CustomTagProcessor::exeDisplayGroup},
    {2, &CustomTagProcessor::exeSoundGroup},
    {3, &CustomTagProcessor::exePictureGroup},
    {4, &CustomTagProcessor::exeFontSizeGroup},
    {5, &CustomTagProcessor::exeLocalizeGroup},
    {6, &CustomTagProcessor::exeNumberGroup},
    {7, &CustomTagProcessor::exeStringGroup},
    {255, &CustomTagProcessor::exeSystemGroup},
    {10, &CustomTagProcessor::exeFontGroup},
    {11, &CustomTagProcessor::exePatchimuGroup},
    {0, nullptr},
};

CustomTagProcessor::Impl::GroupFunctionInfo* CustomTagProcessor::Impl::findGroupFunctionInfo(int group) {
    GroupFunctionInfo* info = sGroupFunctionTable;
    while (info->mFunction) {
        if (info->mGroup == group) {
            return info;
        }

        info++;
    }

    return nullptr;
}

namespace nw4r {
    namespace ut {
        template <>
        f32 TextWriterBase< wchar_t >::PrintImpl(StreamType str, int length) {
            NW4R_POINTER_ASSERT_AT(308, this);
            NW4R_POINTER_ASSERT_AT(309, str);
            NW4R_POINTER_ASSERT_AT(310, GetFont());
            NW4R_MIN_ASSERT_AT(311, length, 0);
            f32 xOrigin = GetCursorX();
            f32 yOrigin = GetCursorY();
            const bool bUseLimit = (mWidthLimit < nw4r::math::F_MAX);
            const f32 orgCursorX = xOrigin;
            const f32 orgCursorY = yOrigin;
            f32 xCursorAdj = 0.0f;
            f32 yCursorAdj = 0.0f;
            f32 textWidth = 0;
            bool bCharSpace = false;
            StreamType prevStreamPos = str;
            StreamType prevNewLinePos = str;

            {
                textWidth = AdjustCursor(&xOrigin, &yOrigin, str, length);
                xCursorAdj = orgCursorX - GetCursorX();
                yCursorAdj = orgCursorY - GetCursorY();
            }

            PrintContext< CharType > context = {this, str, xOrigin, yOrigin, 0};
            CharStrmReader reader = GetFont()->GetCharStrmReader();
            reader.Set(str);

            CustomTagProcessor* processor = nullptr;
            const u8 alpha = GetAlpha();
            if (&mDefaultTagProcessor != mTagProcessor) {
                processor = static_cast< CustomTagProcessor* >(mTagProcessor);
                processor->reset(str);
            }

            for (CharCode code = reader.Next(); reinterpret_cast< StreamType >(reader.GetCurrentPos()) - str <= length;) {
                if (code < ' ')

                {
                    typename TagProcessor::Operation operation;
                    context.str = reinterpret_cast< StreamType >(reader.GetCurrentPos());
                    context.flags = 0;
                    context.flags |= bCharSpace ? 0 : CONTEXT_NO_CHAR_SPACE;

                    if (bUseLimit && (code != '\n') && (prevStreamPos != prevNewLinePos)) {
                        PrintContext< CharType > context2 = context;
                        TextWriterBase< CharType > myCopy = *this;
                        Rect rect;

                        context2.writer = &myCopy;
                        operation = mTagProcessor->CalcRect(&rect, code, &context2);

                        if ((rect.GetWidth() > 0.0f) && (myCopy.GetCursorX() - context.xOrigin > mWidthLimit)) {
                            code = '\n';
                            reader.Set(prevStreamPos);
                            continue;
                        }
                    }

                    operation = mTagProcessor->Process(code, &context);

                    if (operation == TagProcessor::OPERATION_NEXT_LINE) {
                        NW4R_POINTER_ASSERT_AT(378, context.str);

                        if (IsDrawFlagSet(HORIZONTAL_ALIGN_MASK, HORIZONTAL_ALIGN_CENTER)) {
                            const int remain = length - (context.str - str);
                            const f32 width = CalcLineWidth(context.str, remain);
                            const f32 offset = (textWidth - width) / 2;
                            SetCursorX(context.xOrigin + offset);
                        } else if (IsDrawFlagSet(HORIZONTAL_ALIGN_MASK, HORIZONTAL_ALIGN_RIGHT)) {
                            const int remain = length - (context.str - str);
                            const f32 width = CalcLineWidth(context.str, remain);
                            const f32 offset = textWidth - width;
                            SetCursorX(context.xOrigin + offset);
                        } else {
                            const f32 width = GetCursorX() - context.xOrigin;
                            textWidth = Max(textWidth, width);

                            SetCursorX(context.xOrigin);
                        }

                        if (bUseLimit) {
                            prevNewLinePos = reinterpret_cast< StreamType >(reader.GetCurrentPos());
                        }
                        bCharSpace = false;
                    } else if (operation == TagProcessor::OPERATION_NO_CHAR_SPACE) {
                        bCharSpace = false;
                    } else if (operation == TagProcessor::OPERATION_CHAR_SPACE) {
                        bCharSpace = true;
                    } else if (operation == TagProcessor::OPERATION_END_DRAW) {
                        break;
                    }

                    NW4R_POINTER_ASSERT_AT(423, context.str);
                    reader.Set(context.str);
                } else

                {
                    if (processor) {
                        SetAlpha(processor->mAlphaCtrl.alpha());
                        processor->mAlphaCtrl.mCharIndex++;
                    }

                    const f32 baseY = GetCursorY();

                    if (bUseLimit && (prevStreamPos != prevNewLinePos)) {
                        f32 baseX = GetCursorX();
                        f32 charSpace = bCharSpace ? GetCharSpace() : 0.0f;
                        f32 charWidth = IsWidthFixed() ? GetFixedWidth() : GetFont()->GetCharWidth(code) * GetScaleH();

                        if (baseX - xOrigin + charSpace + charWidth > mWidthLimit) {
                            code = '\n';
                            reader.Set(prevStreamPos);
                            continue;
                        }
                    }

                    if (bCharSpace) {
                        MoveCursorX(GetCharSpace());
                    }
                    bCharSpace = true;

                    {
                        const Font* pFont = GetFont();
                        const f32 adj = -pFont->GetBaselinePos() * GetScaleV();
                        MoveCursorY(adj);
                    }

                    CharWriter::Print(code);

                    if (processor) {
                        processor->mPreviousChar = code;
                    }

                    SetCursorY(baseY);
                }

                if (bUseLimit) {
                    prevStreamPos = reinterpret_cast< StreamType >(reader.GetCurrentPos());
                }

                code = reader.Next();
            }

            {
                const f32 width = GetCursorX() - context.xOrigin;
                textWidth = Max(textWidth, width);
            }

            if (IsDrawFlagSet(VERTICAL_ORIGIN_MASK, VERTICAL_ORIGIN_MIDDLE) || IsDrawFlagSet(VERTICAL_ORIGIN_MASK, VERTICAL_ORIGIN_BOTTOM)) {
                SetCursorY(orgCursorY);
            } else {
                MoveCursorY(yCursorAdj);
            }

            if (processor) {
                SetAlpha(alpha);
            }

            return textWidth;
        }
    }  // namespace ut
}  // namespace nw4r

CustomTagProcessor::CustomTagProcessor(nw4r::lyt::TextBox* textBox)
    : mAlphaCtrl(), mPlayedSounds(), mColorMappingMin(setGXColor(textBox->GetMaterial()->GetTevColor(0))),
      mColorMappingMax(setGXColor(textBox->GetMaterial()->GetTevColor(1))) {
    mRubyFontWidth = 0.5f * textBox->mFontSize.width;
    mRubyFontHeight = 0.5f * textBox->mFontSize.height;
    mFontWidth = textBox->mFontSize.width;
    mFontHeight = textBox->mFontSize.height;
    mPreviousChar = 0;
    mIsShadow = false;
    mIsText = false;
    mIsInfo = false;
    mNoCharSpace = false;
    mTextBox = textBox;
}

CustomTagProcessor::Operation CustomTagProcessor::CalcRect(nw4r::ut::Rect* rect, u16 code, ContextType* context) {
    if (isIgnoreTag(code, context)) {
        return MessageTagSkipTagProcessor::CalcRect(rect, code, context);
    }

    if (!mNoCharSpace && !(context->flags & 1)) {
        context->writer->MoveCursorX(context->writer->GetCharSpace());
    }

    MessageEditorMessageTag tag(context);
    Operation operation = OPERATION_DEFAULT;
    Impl::GroupFunctionInfo* info = Impl::findGroupFunctionInfo(tag.getGroup());
    if (info) {
        operation = (this->*info->mFunction)(rect, tag, context);
    }

    context->str += tag.getSkipLength();
    return operation;
}

CustomTagProcessor::Operation CustomTagProcessor::Process(u16 code, ContextType* context) {
    if (isIgnoreTag(code, context)) {
        return MessageTagSkipTagProcessor::Process(code, context);
    }

    if (!mNoCharSpace && !(context->flags & 1)) {
        context->writer->MoveCursorX(context->writer->GetCharSpace());
    }

    MessageEditorMessageTag tag(context);
    Operation operation = OPERATION_DEFAULT;
    Impl::GroupFunctionInfo* info = Impl::findGroupFunctionInfo(tag.getGroup());
    if (info) {
        operation = (this->*info->mFunction)(nullptr, tag, context);
    }

    context->str += tag.getSkipLength();
    return operation;
}

void CustomTagProcessor::setArgNumber(s32 number, s32 index) {
    for (s32 i = 0; i < 4; i++) {
        MessageEditorMessageTag tag = getReplaceTag(mTextBox->mTextBuf, 6, index, i);
        if (!tag.mMessage) {
            break;
        }

        *reinterpret_cast< s32* >(tag.getParamPtr(0)) = number;
    }
}

void CustomTagProcessor::setArgString(const wchar_t* string, s32 index) {
    for (s32 i = 0; i < 4; i++) {
        MessageEditorMessageTag tag = getReplaceTag(mTextBox->mTextBuf, 7, index, i);
        if (!tag.mMessage) {
            break;
        }

        *reinterpret_cast< const wchar_t** >(tag.getParamPtr(0)) = string;
    }
}

void CustomTagProcessor::initAlpha(f32 speed, f32 interval, s32 startDelay, s32 endDelay) {
    mAlphaCtrl.init(MR::countMessageChar(mTextBox->mTextBuf), speed, interval, startDelay, endDelay);
    mPlayedSounds = 0;
    mSoundIndex = 0;
    mColorIndex = 0;
}

void CustomTagProcessor::reset(const wchar_t* string) {
    if (mTextBox->mTextBuf != string) {
        return;
    }

    mSoundIndex = 0;
    mColorIndex = 0;
    mAlphaCtrl.mWaitTime = 0;
    mAlphaCtrl.mCharIndex = 0;
    mPreviousChar = 0;
}

MessageEditorMessageTag CustomTagProcessor::getReplaceTag(const wchar_t* string, s32 group, s32 index, s32 occurrence) const {
    while (*string) {
        if (*string == 0x1A) {
            string++;
            MessageEditorMessageTag tag(string);
            string += tag.getSkipLength();
            s32 tagGroup = tag.getGroup();
            if (tagGroup == group && index == tag.getParam32(1)) {
                if (occurrence == 0) {
                    return tag;
                }

                occurrence--;
            }
        } else {
            string++;
        }
    }

    return MessageEditorMessageTag(static_cast< const wchar_t* >(nullptr));
}

bool CustomTagProcessor::isIgnoreTag(u16 code, ContextType* context) const {
    if (code != 0x1A) {
        return true;
    }

    MessageEditorMessageTag tag(context);
    return !Impl::findGroupFunctionInfo(tag.getGroup());
}

bool CustomTagProcessor::writeString(nw4r::ut::Rect* rect, const wchar_t* string, ContextType* context) {
    nw4r::ut::TextWriterBase< wchar_t >* writer = context->writer;
    if (!string) {
        return false;
    }

    if (!rect) {
        writer->MoveCursorY(-writer->GetFontAscent());
        writer->Print(string, MR::getStringLengthWithMessageTag(string));
        writer->MoveCursorY(writer->GetFontAscent());
    } else {
        nw4r::ut::TextWriterBase< wchar_t > copy = *writer;
        f32 x = copy.GetCursorX();
        f32 y = copy.GetCursorY();
        copy.CalcStringRect(rect, string, MR::getStringLengthWithMessageTag(string));
        context->writer->MoveCursorX(rect->GetWidth());
        context->writer->MoveCursorY(rect->GetHeight() - copy.GetFontHeight());
        rect->left += x;
        rect->right += x;
        rect->top += y;
        rect->bottom += y;
    }

    return true;
}

CustomTagProcessor::Operation CustomTagProcessor::exeDisplayGroup(nw4r::ut::Rect* rect, const MessageEditorMessageTag& tag, ContextType* context) {
    switch (tag.getTag()) {
    case 0:
        return exeDisplayGroupWait(rect, tag.getParam16(0), context);
    case 2:
        return exeDisplayGroupOffset(rect, tag, context);
    case 3:
        return exeDisplayGroupCenter(rect, tag, context);
    case 1:
        return OPERATION_END_DRAW;
    }

    return OPERATION_NO_CHAR_SPACE;
}

CustomTagProcessor::Operation CustomTagProcessor::exeSoundGroup(nw4r::ut::Rect* rect, const MessageEditorMessageTag& tag, ContextType* context) {
    if (rect || mIsText || !mAlphaCtrl.alpha()) {
        return OPERATION_NO_CHAR_SPACE;
    }

    u32 mask = static_cast< u8 >(1 << mSoundIndex);
    if (mask != (mPlayedSounds & mask)) {
        char name[256];
        s32 length = static_cast< s32 >(tag.getParamLength()) / 2;
        MR::convertUTF16ToASCII(name, tag.getParamPtr(0), length + 1);
        MR::startSystemSE(name, -1, -1);
        mPlayedSounds |= static_cast< u8 >(1 << mSoundIndex);
    }

    mSoundIndex++;
    return OPERATION_NO_CHAR_SPACE;
}

CustomTagProcessor::Operation CustomTagProcessor::exePictureGroup(nw4r::ut::Rect* rect, const MessageEditorMessageTag& tag, ContextType* context) {
    nw4r::ut::TextWriterBase< wchar_t > writer = *context->writer;
    writer.SetFont(*MR::getPictureFontNW4R());
    wchar_t picture = tag.getTag() + L'0';
    f32 width = writer.CalcStringWidth(&picture, 1) + writer.GetCharSpace();
    context->writer->MoveCursorX(width);

    if (rect) {
        rect->right = rect->left + width;
        rect->SetHeight(writer.GetFontHeight() + writer.GetLineSpace());
    } else {
        if (!mIsShadow) {
            writer.ResetColorMapping();
            writer.SetupGX();
            if (mColorIndex) {
                setTextColor(&writer, 0);
            }
        }

        writer.MoveCursorY(mPictureFontOffset + -writer.GetFontAscent());
        writer.Print(&picture, 1);
        if (!mIsShadow) {
            context->writer->SetupGX();
        }
    }

    return OPERATION_DEFAULT;
}

CustomTagProcessor::Operation CustomTagProcessor::exeFontSizeGroup(nw4r::ut::Rect* rect, const MessageEditorMessageTag& tag, ContextType* context) {
    nw4r::ut::TextWriterBase< wchar_t >* writer = context->writer;
    switch (tag.getTag()) {
    case 0:
        writer->SetFontSize(0.75f * mFontWidth, 0.75f * mFontHeight);
        break;
    case 1:
        writer->SetFontSize(mFontWidth, mFontHeight);
        break;
    case 2:
        writer->SetFontSize(1.5f * mFontWidth, 1.5f * mFontHeight);
        break;
    }

    switch (tag.getTag()) {
    case 2:
        if (mTextBox->GetTextPositionV() != 0) {
            writer->MoveCursorY(mFontSizeOffset * mFontHeight);
        }
        break;
    }

    return OPERATION_NO_CHAR_SPACE;
}

CustomTagProcessor::Operation CustomTagProcessor::exeSystemGroup(nw4r::ut::Rect* rect, const MessageEditorMessageTag& tag, ContextType* context) {
    switch (tag.getTag()) {
    case 0:
        return exeSystemGroupColor(rect, tag.getParam8(0), context);
    case 2:
        return exeSystemGroupRuby(rect, tag, context);
    }

    return OPERATION_NO_CHAR_SPACE;
}

CustomTagProcessor::Operation CustomTagProcessor::exeLocalizeGroup(nw4r::ut::Rect* rect, const MessageEditorMessageTag& tag, ContextType* context) {
    wchar_t string[32];
    ReplaceTagProcessor::exeLocalizeGroup(string, tag);
    writeString(rect, string, context);
    return OPERATION_DEFAULT;
}

CustomTagProcessor::Operation CustomTagProcessor::exeNumberGroup(nw4r::ut::Rect* rect, const MessageEditorMessageTag& tag, ContextType* context) {
    wchar_t string[16];
    s32 number = *reinterpret_cast< s32* >(tag.getParamPtr(0));
    switch (tag.getTag()) {
    case 5:
        swprintf(string, 256, L"%02d", number);
    case 6:
        swprintf(string, 256, L"%03d", number);
    case 7:
        swprintf(string, 256, L"%04d", number);
    case 8:
        swprintf(string, 256, L"%05d", number);
    case 9:
        swprintf(string, 256, L"%06d", number);
    default:
        swprintf(string, 256, L"%d", number);
    }

    writeString(rect, string, context);
    return OPERATION_DEFAULT;
}

CustomTagProcessor::Operation CustomTagProcessor::exeStringGroup(nw4r::ut::Rect* rect, const MessageEditorMessageTag& tag, ContextType* context) {
    if (!*reinterpret_cast< const u8* >(tag.getParamPtr(0))) {
        return OPERATION_DEFAULT;
    }

    writeString(rect, *reinterpret_cast< const wchar_t** >(tag.getParamPtr(0)), context);
    return OPERATION_DEFAULT;
}

CustomTagProcessor::Operation CustomTagProcessor::exeSystemGroupColor(nw4r::ut::Rect* rect, int index, ContextType* context) {
    if (mIsShadow) {
        return OPERATION_NO_CHAR_SPACE;
    }

    if (mIsInfo && index >= 1 && index < 6) {
        index += 5;
    }

    if (!rect) {
        mColorIndex = index;
        if (index == 0) {
            context->writer->SetColorMapping(mColorMappingMin, mColorMappingMax);
        } else {
            GXColor color = getTextColor(index);
            color.a = mColorMappingMin.a;
            context->writer->SetColorMapping(color, getTextColor(index));
        }

        context->writer->SetupGX();
        setTextColor(context->writer, index);
    }

    return OPERATION_NO_CHAR_SPACE;
}

CustomTagProcessor::Operation CustomTagProcessor::exeSystemGroupRuby(nw4r::ut::Rect* rect, const MessageEditorMessageTag& tag, ContextType* context) {
    if (MR::getLanguage() != 0x10) {
        return OPERATION_NO_CHAR_SPACE;
    }

    s32 baseLength = tag.getParam8(0);
    const wchar_t* rubyText = tag.getParamPtr(1);
    s32 rubyLength = (tag.getParamLength() - 2) / sizeof(wchar_t);
    wchar_t ruby[32];
    MR::copyMemory(ruby, rubyText, rubyLength * sizeof(wchar_t));
    ruby[rubyLength] = L'\0';
    wchar_t base[32];
    MR::copyMemory(base, context->str + tag.getTagLength() / 2, baseLength * sizeof(wchar_t));
    base[baseLength] = L'\0';
    nw4r::ut::TextWriterBase< wchar_t >* writer = context->writer;
    writer->GetDrawFlag();
    writer->GetCharSpace();
    nw4r::ut::TextWriterBase< wchar_t > rubyWriter = *writer;
    rubyWriter.SetDrawFlag(nw4r::ut::TextWriterBase< wchar_t >::VERTICAL_ORIGIN_BASELINE);
    rubyWriter.SetLineSpace(0.0f);
    rubyWriter.SetCharSpace(mRubyCharSpace);
    rubyWriter.SetFontSize(mRubyFontWidth, mRubyFontHeight);
    rubyWriter.SetTagProcessor(this);
    f32 baseWidth = writer->CalcStringWidth(base, baseLength);
    f32 rubyWidth = rubyWriter.CalcStringWidth(ruby, rubyLength);
    if (!rect) {
        f32 extraWidth = baseWidth - rubyWidth;
        if (extraWidth > 0.0f) {
            f32 space = extraWidth / (rubyLength + 1);
            rubyWriter.MoveCursorX(space);
            rubyWriter.SetCharSpace(mRubyCharSpace + space);
        } else {
            rubyWriter.MoveCursorX(extraWidth / 2.0f);
        }

        rubyWriter.MoveCursorY(mRubyBaseLineOffset + -writer->GetFontAscent());
        CustomTagAlphaCtrl alpha = mAlphaCtrl;
        s32 index = rubyLength * alpha.mCharIndex / baseLength;
        f32 interval = (baseLength * alpha.mCharInterval) / rubyLength;
        mAlphaCtrl.mCharInterval = interval;
        mAlphaCtrl.mCharIndex = index;
        rubyWriter.Print(ruby, rubyLength);
        mAlphaCtrl = alpha;
    }

    return OPERATION_NO_CHAR_SPACE;
}

CustomTagProcessor::Operation CustomTagProcessor::exeDisplayGroupWait(nw4r::ut::Rect* rect, u16 frames, ContextType* context) {
    if (!rect) {
        mAlphaCtrl.mWaitTime += frames;
    }

    return OPERATION_NO_CHAR_SPACE;
}

CustomTagProcessor::Operation CustomTagProcessor::exeDisplayGroupOffset(nw4r::ut::Rect* rect, const MessageEditorMessageTag& tag,
                                                                        ContextType* context) {
    nw4r::ut::TextWriterBase< wchar_t >* writer = context->writer;
    const wchar_t* string = context->str + tag.getSkipLength();
    nw4r::ut::Rect bounds;
    if (mTextBox->GetTextPositionV() == 0) {
        writer->CalcStringRect(&bounds, string, MR::getStringLengthWithMessageTag(string));
        writer->MoveCursorY((mTextBox->mSize.height - bounds.GetHeight()) / 2.0f);
    }

    return OPERATION_NO_CHAR_SPACE;
}

CustomTagProcessor::Operation CustomTagProcessor::exeDisplayGroupCenter(nw4r::ut::Rect* rect, const MessageEditorMessageTag& tag,
                                                                        ContextType* context) {
    nw4r::ut::TextWriterBase< wchar_t > writer = *context->writer;
    const wchar_t* string = context->str + tag.getSkipLength();
    nw4r::ut::Rect bounds;
    writer.CalcStringRect(&bounds, string, MR::getStringLengthWithMessageTag(string));
    f32 offset = (mTextBox->mSize.width - bounds.GetWidth()) / 2.0f;
    context->writer->MoveCursorX(offset);
    context->xOrigin += offset;
    return OPERATION_NO_CHAR_SPACE;
}

CustomTagProcessor::Operation CustomTagProcessor::exeFontGroup(nw4r::ut::Rect* rect, const MessageEditorMessageTag& tag, ContextType* context) {
    nw4r::ut::TextWriterBase< wchar_t > writer = *context->writer;
    writer.SetFont(*MR::getNumberFontNW4R());
    const wchar_t* string = tag.getParamPtr(0);
    s32 length = static_cast< s32 >(tag.getParamLength()) / 2;
    f32 width = writer.CalcStringWidth(string, length) + writer.GetCharSpace();
    context->writer->MoveCursorX(width);
    if (rect) {
        rect->right = rect->left + width;
        rect->SetHeight(writer.GetFontHeight() + writer.GetLineSpace());
    } else {
        f32 offset = -writer.GetFontAscent();
        if (!mIsShadow) {
            writer.ResetColorMapping();
            writer.SetupGX();
            setTextColor(&writer, 0);
        }

        writer.MoveCursorY(offset);
        writer.Print(string, length);
        if (!mIsShadow) {
            context->writer->SetupGX();
        }
    }

    return OPERATION_NO_CHAR_SPACE;
}

CustomTagProcessor::Operation CustomTagProcessor::exePatchimuGroup(nw4r::ut::Rect* rect, const MessageEditorMessageTag& tag, ContextType* context) {
    wchar_t string[16] = {};
    wchar_t* out = string;
    bool hasPatchim = true;
    if (mPreviousChar >= L'0' && mPreviousChar < L'9') {
        if (mPreviousChar == L'3' || mPreviousChar == L'6') {
            hasPatchim = true;
        } else {
            hasPatchim = false;
        }
    } else if (mPreviousChar >= 0xAC00) {
        int patchim = (mPreviousChar - 0xAC00) % 28;
        hasPatchim = patchim != 0;
        if (tag.getTag() == 4 && patchim == 8) {
            hasPatchim = false;
        }
    }

    switch (tag.getTag()) {
    case 0:
        *out++ = hasPatchim ? 0xC740 : 0xB294;
        break;
    case 1:
        *out++ = hasPatchim ? 0xC744 : 0xB97C;
        break;
    case 2:
        *out++ = hasPatchim ? 0xC774 : 0xAC00;
        break;
    case 3:
        *out++ = hasPatchim ? 0xACFC : 0xC640;
        break;
    case 4:
        if (hasPatchim) {
            *out++ = 0xC73C;
        } else {
            return OPERATION_NO_CHAR_SPACE;
        }
        break;
    case 5:
        *out++ = hasPatchim ? 0xBEC6 : 0xBEDF;
        break;
    case 6:
        if (hasPatchim) {
            *out++ = 0xC774;
        } else {
            return OPERATION_NO_CHAR_SPACE;
        }
        break;
    }

    *out = L'\0';
    writeString(rect, string, context);
    return OPERATION_DEFAULT;
}

CustomTagProcessor::~CustomTagProcessor() {
}
