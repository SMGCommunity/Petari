#pragma once

#include "nw4r/ut/Rect.hpp"
#include <revolution.h>

namespace nw4r {
    namespace ut {

        template <typename T>
        class TextWriterBase;

        template<typename T>
        class PrintContext {
            TextWriterBase<T>* mWriter; // 0x0
            const T* mString;           // 0x4
            f32 mXOrigin;               // 0x8
            f32 mYOrigin;               // 0xC
            u32 mFlags;                 // 0x10
        };
    
        template<typename T>
        class TagProcessorBase {
        public:

            typedef PrintContext<T> ContextType;

            enum Operation
            {
                OPERATION_DEFAULT,
                OPERATION_NO_CHAR_SPACE,
                OPERATION_CHAR_SPACE,
                OPERATION_NEXT_LINE,
                OPERATION_END_DRAW,
                NUM_OF_OPERATION
            };

            TagProcessorBase();

            virtual ~TagProcessorBase();
            virtual Operation Process(u16, PrintContext<T> *);
            virtual Operation CalcRect(Rect *, u16, PrintContext<T> *);
        };
    };
};