#pragma once

#include <revolution.h>

template < typename T >
class JASGlobalInstance {
public:
    static T* sInstance;

    JASGlobalInstance(bool setInstance) {
        if (setInstance) {
            sInstance = (T*)this;
        }
    }

    ~JASGlobalInstance() {
        if (sInstance == (T*)this) {
            sInstance = nullptr;
        }
    }

    static T* getInstance() {
        return sInstance;
    }
};

template < typename T >
T* JASGlobalInstance< T >::sInstance;
