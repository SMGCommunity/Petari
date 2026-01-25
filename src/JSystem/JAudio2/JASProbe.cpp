#include "JSystem/JAudio2/JASProbe.hpp"
#include "JSystem/JAudio2/JASTrack.hpp"
#include <revolution/os.h>

JASProbe* JASProbe::sProbeTable[16] = {nullptr};

void JASProbe::start(char const* name) {
    JASCriticalSection cs;
    mName = name;
    mStartTime = OSGetTime();
}

void JASProbe::stop() {
    JASCriticalSection cs;
    u32 endTime = OSGetTime();
    _8 = endTime - mStartTime;
    _8 /= OS_TIMER_CLOCK / 59.94f;
    if (_10 < _8 && 100 < _1A8) {
        _10 = _8;
    }
    _C = _C * 0.96f + _8 * 0.04f;
    u32 resultIndex = _1A8 % 100;
    _14 -= _18[resultIndex];
    _18[resultIndex] = _8;
    _14 += _8;
    _1A8++;
}

void JASProbe::start(s32 index, char const* name) {
    if (index < 16) {
        JASProbe* probe = sProbeTable[index];
        if (probe != nullptr) {
            probe->start(name);
        }
    }
}

void JASProbe::stop(s32 index) {
    if (index < 16) {
        JASProbe* probe = sProbeTable[index];
        if (probe != nullptr) {
            probe->stop();
        }
    }
}
