#include "Game/NWC24/UTF16Util.hpp"

namespace MR {
    size_t strlenUTF16(const u16* pStr) {
        for (int len = 0; len < 1024; len++) {
            if (*pStr++ != '\0') {
                continue;
            }

            return len;
        }

        return 0;
    }
}; // namespace MR
