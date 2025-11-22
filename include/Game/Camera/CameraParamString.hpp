#pragma once

class CameraParamString {
public:
    CameraParamString();

    CameraParamString& operator=(const CameraParamString&);

    void setCharPtr(const char*);
    const char* getCharPtr() const;
    void copy(const char*);

    const char* mString;  // 0x0
};