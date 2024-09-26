#pragma once

template<typename T>
class SingletonHolder {
public:
    static T* get() {
        return sInstance;
    }

    static void set(T *p) {
        sInstance = p;
    }

    static bool exists() {
        return sInstance != nullptr;
    }

    static T* sInstance;
};

template<typename T>
T* SingletonHolder<T>::sInstance;