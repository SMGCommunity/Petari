#pragma once

class J2DDataManage;

class J2DScreen {
public:
    static void* getNameResource(char const*);

    static J2DDataManage* getDataManage() { return mDataManage; }

    static J2DDataManage* mDataManage;
};
