#pragma once

#include "Game/Screen/MessageTagSkipTagProcessor.hpp"
#include <nw4r/lyt/textBox.h>
#include <nw4r/ut/TagProcessorBase.h>
#include <revolution/gx/GXStruct.h>

namespace {
    u8 clampU8(s32 val) {
        if (val < 0) {
            return 0;
        }

        u8 ret = 0xFF;
        if (val <= 255) {
            ret = val;
        }

        return ret;
    }
};  // namespace

class CustomTagAlphaCtrl {
public:
    CustomTagAlphaCtrl();

    void init(u32, f32, f32, s32, s32);
    u8 alpha() const;
    void update();
    bool isEnd() const;

    /* 0x00 */ bool _0;
    /* 0x01 */ bool _1;
    /* 0x02 */ bool _2;
    /* 0x04 */ u32 _4;
    /* 0x08 */ u32 _8;
    /* 0x0C */ u32 _C;
    /* 0x10 */ GXColorS10 _10;
    /* 0x18 */ u8 _18;
    /* 0x1C */ f32 _1C;
    /* 0x20 */ f32 _20;
};

class CustomTagProcessor : public MessageTagSkipTagProcessor {
public:
    CustomTagProcessor(nw4r::lyt::TextBox*);

    virtual nw4r::ut::TagProcessorBase< wchar_t >::Operation Process(u16, nw4r::ut::PrintContext< wchar_t >*);
    virtual nw4r::ut::TagProcessorBase< wchar_t >::Operation CalcRect(nw4r::ut::Rect*, u16, nw4r::ut::PrintContext< wchar_t >*);

    /* 0x04 */ CustomTagAlphaCtrl mAlphaCtrl;
    /* 0x28 */ u8 _28[0x28];
};
