#include "JSystem/J3DGraphLoader/J3DModelLoader.hpp"

J3DModelLoader::J3DModelLoader() {
    mpModelData = nullptr;
    mpMaterialTable = nullptr;
    mpShapeBlock = nullptr;
    mpMaterialBlock = nullptr;
    mpModelHierarchy = nullptr;
    _18 = 0;
    mEnvelopeSize = 0;
}

J3DModelData* J3DModelLoaderDataBase::load(const void* data, u32 flags) {
    if (data == nullptr) {
        return nullptr;
    }

    if (*(u32*)data == 'J3D1' && *(u32*)((u32)data + 4) == 'bmd1') {
        return nullptr;
    }

    if (*(u32*)data == 'J3D2' && *(u32*)((u32)data + 4) == 'bmd2') {
        J3DModelLoader_v21 loader;
        return loader.load(data, flags);
    }

    if (*(u32*)data == 'J3D2' && *(u32*)((u32)data + 4) == 'bmd3') {
        J3DModelLoader_v26 loader;
        return loader.load(data, flags);
    }

    return nullptr;
}

// J3DModelLoaderDataBase::loadMaterialTable
// J3DModelLoaderDataBase::loadBinaryDisplayList
