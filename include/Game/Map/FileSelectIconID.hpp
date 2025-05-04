#pragma once

#include <revolution.h>

class FileSelectIconID {
public:
    enum EFellowID {
        Mario,
        Luigi,
        Yoshi,
        Kinopio,
        Peach,
    };

    FileSelectIconID();
    FileSelectIconID(const FileSelectIconID&);

    void set(const FileSelectIconID&);
    void setMiiIndex(u16);
    bool isMii() const;
    u16 getMiiIndex() const;
    void setFellowID(EFellowID);
    bool isFellow() const;
    EFellowID getFellowID() const;

private:
    bool mIsMii;
    u16 mData;
};
