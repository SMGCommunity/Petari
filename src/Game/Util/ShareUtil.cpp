#include "Game/Util/ShareUtil.hpp"

ResourceShare::~ResourceShare() {
}

ResourceShare::ResourceShare() : NameObj("資源共有機構") {
    _C = new u8[0x80];
    _10 = new u8[0x80];
    _14 = 0;
}
