#include "JSystem/JAudio2/JAISoundChild.hpp"
#include "JSystem/JAudio2/JASTrack.hpp"

void JAISoundChild::init() {
    mMove.init();
    mParams.init();
}

void JAISoundChild::mixOut(JASTrack* track) {
    mParams = mMove.mParams;
    track->assignExtBuffer(0, &mParams);
}

void JAISoundChild::calc() {
    mMove.calc();
}
