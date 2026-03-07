#include "JSystem/J2DGraph/J2DScreen.hpp"
#include "JSystem/J2DGraph/J2DManage.hpp"
#include "JSystem/JKernel/JKRFileLoader.hpp"

J2DDataManage* J2DScreen::mDataManage;

void* J2DScreen::getNameResource(char const* resName) {
    void* res = JKRFileLoader::getGlbResource(resName, NULL);

    if (res == NULL && mDataManage != NULL) {
        res = mDataManage->get(resName);
    }

    return res;
}
