#pragma once

#include <JSystem/JAudio2/JAUSoundMgr.hpp>
#include <JSystem/JAudio2/JASAudioReseter.hpp>

class JAUSectionHeap;
class JKRArchive;

class AudSystem : public JAUSoundMgr {
public:

    inline void setVar(u32 var) {
        _830 = var;
    }

    void enterPauseMenu();
    void exitPauseMenu();

    u8 _814;
    u8 _815;
    u8 _816;
    JASAudioReseter mReseter;       // 0x818
    u8 _828;
    u8 _829;
    u8 _82A;
    u8 _82B;
    u8 _82C;
    u32 _830;
    u32 _834;
    u32 _838;
    u32 _83C;
};
