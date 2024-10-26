#include "Game/Map/FileSelectIconID.hpp"

FileSelectIconID::FileSelectIconID() {
    _0 = false;
    _2 = 0;
}

FileSelectIconID::FileSelectIconID(const FileSelectIconID &rOther) {
    _0 = rOther._0;
    _2 = rOther._2;
}

void FileSelectIconID::set(const FileSelectIconID &rOther) {
    _0 = rOther._0;
    _2 = rOther._2;
}

void FileSelectIconID::setMiiIndex(u16 idIdx) {
    _2 = idIdx;
    _0 = true;
}

bool FileSelectIconID::isMii() const {
    return _0;
}

u16 FileSelectIconID::getMiiIndex() const {
    return _2;
}

void FileSelectIconID::setFellowID(FileSelectIconID::EFellowID fellowID) {
    _2 = (u16)fellowID;
    _0 = 0;
}

bool FileSelectIconID::isFellow() const {
    return !_0;
}

FileSelectIconID::EFellowID FileSelectIconID::getFellowID() const {
    return (EFellowID)_2;
}
