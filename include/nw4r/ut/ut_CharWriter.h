#pragma once

#include "smg.h"
#include "nw4r/ut/ut_Color.h"

namespace nw4r
{
    namespace ut
    {
        class CharWriter
        {
        public:
            struct ColorMapping
            {
                Color _0;
                Color _4;
            };


            static inline void Setup();
            static inline void SetupVtx();
            static void SetupGXWithColorMapping(Color color_0, Color color_1);
        };
    };
};