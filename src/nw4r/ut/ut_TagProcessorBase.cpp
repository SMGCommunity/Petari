#include "nw4r/ut/CharWriter.h"
#include "nw4r/ut/TagProcessorBase.h"
#include "nw4r/ut/TextWriterBase.h"

namespace nw4r {
    namespace ut {
        template < typename CharType >
        TagProcessorBase< CharType >::TagProcessorBase() {}

        template < typename CharType >
        TagProcessorBase< CharType >::~TagProcessorBase() {}

        template < typename CharType >
        typename TagProcessorBase< CharType >::Operation TagProcessorBase< CharType >::Process(u16 code, ContextType* context) {
            switch (code) {
            case '\n': {
                ProcessLinefeed(context);
            }
                return OPERATION_NEXT_LINE;

            case '\t': {
                ProcessTab(context);
            }
                return OPERATION_NO_CHAR_SPACE;

            default: {
                // do nothing
            } break;
            }

            return OPERATION_DEFAULT;
        }

        template < typename CharType >
        typename TagProcessorBase< CharType >::Operation TagProcessorBase< CharType >::CalcRect(Rect* pRect, u16 code, ContextType* context) {
            switch (code) {
            case '\n': {
                TextWriterBase< CharType >& writer = *context->writer;
                pRect->right = writer.GetCursorX();
                pRect->top = writer.GetCursorY();
                ProcessLinefeed(context);
                pRect->left = writer.GetCursorX();
                pRect->bottom = writer.GetCursorY() + context->writer->GetFontHeight();
                pRect->Normalize();
            }
                return OPERATION_NEXT_LINE;

            case '\t': {
                TextWriterBase< CharType >& writer = *context->writer;
                pRect->left = writer.GetCursorX();
                ProcessTab(context);
                pRect->right = writer.GetCursorX();
                pRect->top = writer.GetCursorY();
                pRect->bottom = pRect->top + writer.GetFontHeight();
                pRect->Normalize();
            }
                return OPERATION_NO_CHAR_SPACE;

            default: {
                // do nothing
            } break;
            }

            return OPERATION_DEFAULT;
        }

        template < typename T >
        void TagProcessorBase< T >::ProcessTab(ContextType* pCtx) {
            TextWriterBase< T >& rWriter = *pCtx->writer;

            int tabWidth = rWriter.GetTabWidth();
            if (tabWidth <= 0) {
                return;
            }

            f32 charWidth = rWriter.IsWidthFixed() ? rWriter.GetFixedWidth() : rWriter.GetFontWidth();

            f32 dx = rWriter.GetCursorX() - pCtx->xOrigin;
            f32 tabPixel = tabWidth * charWidth;
            int numTab = static_cast< int >(dx / tabPixel) + 1;
            f32 x = pCtx->xOrigin + (tabPixel * numTab);

            rWriter.SetCursorX(x);
        }

        template < typename CharType >
        void TagProcessorBase< CharType >::ProcessLinefeed(ContextType* context) {
            TextWriterBase< CharType >& writer = *context->writer;
            const f32 x = context->xOrigin;
            const f32 y = writer.GetCursorY() + writer.GetLineHeight();
            writer.SetCursor(x, y);
        }

        template class TagProcessorBase< char >;
        template class TagProcessorBase< wchar_t >;
    };  // namespace ut
};  // namespace nw4r
