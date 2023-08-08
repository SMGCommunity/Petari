#include "JSystem/JAudio2/JASTrackPort.h"

u16 JASTrackPort::readImport(u32 port) {
    mImport &= ~(1 << port);
    return mPorts[port];
}

u16 JASTrackPort::readExport(u32 port) {
    mExport &= ~(1 << port);
    return mPorts[port];
}

void JASTrackPort::writeImport(u32 port, u16 value) {
    mImport |= 1 << port;
    mPorts[port] = value;
}

void JASTrackPort::writeExport(u32 port, u16 value) {
    mExport |= 1 << port;
    mPorts[port] = value;
}

bool JASTrackPort::checkImport(u32 port) const {
    return mImport & (1 << port);
}

bool JASTrackPort::checkExport(u32 port) const {
    return mExport & (1 << port);
}
