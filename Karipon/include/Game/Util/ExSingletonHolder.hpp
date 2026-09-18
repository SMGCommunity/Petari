#pragma once

#include "revolution/types.h"

template <typename T>
class ExSingletonHolder {
public:
    static T* init() {
        if (sInstance == nullptr) {
            sInstance = new T();
        }

        return sInstance;
    }

    static T* get() { return sInstance; }

public:
    static T* sInstance;
};

template <typename T>
T* ExSingletonHolder<T>::sInstance;
