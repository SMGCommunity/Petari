#pragma once

template < typename T >
class SingletonHolder {
public:
    static void init() {
        if (sInstance == nullptr) {
            sInstance = new T();
        }
    }

    static T* get() {
        return sInstance;
    }

private:
    static T* sInstance;
};

template < typename T >
T* SingletonHolder< T >::sInstance;

template < typename T >
class AudSingletonHolder {
public:
    static void init() {
        if (sInstance == nullptr) {
            sInstance = new T();
        }
    }

    static T* get() {
        return sInstance;
    }

private:
    static T* sInstance;
};

template < typename T >
T* AudSingletonHolder< T >::sInstance;
