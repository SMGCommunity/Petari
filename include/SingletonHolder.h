#ifndef SINGLETONHOLDER_H
#define SINGLETONHOLDER_H

template<typename T>
class SingletonHolder 
{
public:
    static T* sInstance;
};

#endif // SINGLETONHOLDER_H