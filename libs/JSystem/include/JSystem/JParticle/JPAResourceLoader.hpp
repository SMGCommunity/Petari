#pragma once

#include <revolution/types.h>

class JPAResourceManager;

class JPAResourceLoader {
public:
    JPAResourceLoader(u8 const*, JPAResourceManager*);
    void load_jpc(u8 const*, JPAResourceManager*);
};
