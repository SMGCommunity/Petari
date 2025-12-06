#pragma once

#include <revolution.h>

class JASDsp {
public:
    struct TChannel {
        void setIIRFilterParam(s16*);
        void setFIR8FilterParam(s16*);
        void setFilterMode(u16);
        void setDistFilter(s16);
    };
};