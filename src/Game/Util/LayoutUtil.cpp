#include "Game/Util/LayoutUtil.hpp"

namespace MR {
    void setTextBoxNumberRecursive(LayoutActor *pActor, const char *pText, s32 a3) {
        MR::setTextBoxFormatRecursive(pActor, pText, L"%d", a3);
    }

    void clearTextBoxMessageRecursive(LayoutActor *pActor, const char *pText) {
        MR::setTextBoxMessageRecursive(pActor, pText, L"");
    }
};