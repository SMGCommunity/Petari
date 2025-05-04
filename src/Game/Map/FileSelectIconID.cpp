#include "Game/Map/FileSelectIconID.hpp"

FileSelectIconID::FileSelectIconID() :
    mIsMii(false),
    mData(0)
{}

FileSelectIconID::FileSelectIconID(const FileSelectIconID& rOther) {
    mIsMii = rOther.mIsMii;
    mData = rOther.mData;
}

void FileSelectIconID::set(const FileSelectIconID& rOther) {
    mIsMii = rOther.mIsMii;
    mData = rOther.mData;
}

void FileSelectIconID::setMiiIndex(u16 miiIndex) {
    mData = miiIndex;
    mIsMii = true;
}

bool FileSelectIconID::isMii() const {
    return mIsMii;
}

u16 FileSelectIconID::getMiiIndex() const {
    return mData;
}

void FileSelectIconID::setFellowID(EFellowID fellowID) {
    mData = fellowID;
    mIsMii = false;
}

bool FileSelectIconID::isFellow() const {
    return !mIsMii;
}

FileSelectIconID::EFellowID FileSelectIconID::getFellowID() const {
    return static_cast<EFellowID>(mData);
}
