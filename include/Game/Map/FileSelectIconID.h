#pragma once

#include <revolution.h>

class FileSelectIconID {
public:
    enum EFellowID {

    };

    FileSelectIconID();
    FileSelectIconID(const FileSelectIconID &);

    void set(const FileSelectIconID &);
    void setMiiIndex(u16);
    bool isMii() const;
    u16 getMiiIndex() const;
    void setFellowID(FileSelectIconID::EFellowID);
    bool isFellow() const;
    FileSelectIconID::EFellowID getFellowID() const;

    bool _0;
    u16 _2;
};
