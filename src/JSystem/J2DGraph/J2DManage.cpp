#include "JSystem/J2DGraph/J2DManage.hpp"
#include <cstdio>

void* J2DDataManage::get(char const* name) {
    for (J2DataManageLink* link = mList; link != NULL; link = link->mNext) {
        if (strcmp(link->mName, name) == 0) {
            return link->mData;
        }
    }
    return NULL;
}
