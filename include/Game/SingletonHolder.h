#pragma once

template<typename T>
class SingletonHolder {
public:
    static T* sInstance;
};

template<typename T>
T* SingletonHolder<T>::sInstance;