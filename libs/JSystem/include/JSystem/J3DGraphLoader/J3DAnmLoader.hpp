#pragma once

enum J3DAnmLoaderDataBaseFlag {

};

class J3DAnmLoaderDataBase {
public:
    static void* load(const void *, J3DAnmLoaderDataBaseFlag);
    static void* loadMaterialTable(const void *);
};