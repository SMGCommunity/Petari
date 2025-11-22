#pragma once

template < class T >
class JASGlobalInstance {
public:
    JASGlobalInstance(T* inst) { sInstance = inst; }
    static T* sInstance;
};