#pragma once

#include "JSystem/JAudio2/JASSeqReader.hpp"

class AudMeSeqReader : public JASSeqReader {
public:
    AudMeSeqReader() {
        init();
    }

    void init() {
        init(nullptr);
    }

    void init(void*);
    bool call(u32);
    bool loopStart(u16);
    bool loopEnd();
    bool ret();
};
