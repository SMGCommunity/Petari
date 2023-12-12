#pragma once

#include "Game/Screen/MessageTagSkipTagProcessor.h"

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
};

struct CustomTagArg {

};

class CustomTagProcessor : public MessageTagSkipTagProcessor {
public:

};