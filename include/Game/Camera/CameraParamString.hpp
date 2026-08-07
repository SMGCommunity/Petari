#pragma once

class CameraParamString {
public:
    CameraParamString();

    CameraParamString& operator=(const CameraParamString&);

    void setCharPtr(const char*);
    const char* getCharPtr() const;
    void copy(const char*);

    /* 0x00 */ const char* mString;
};
