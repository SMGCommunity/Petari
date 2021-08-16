#pragma once

template<typename T>
class SingletonHolder {
public:
    static T* sInstance;
};
