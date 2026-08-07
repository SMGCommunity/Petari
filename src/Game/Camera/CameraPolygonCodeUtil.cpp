#include "Game/Camera/CameraPolygonCodeUtil.hpp"
#include "Game/Camera/CameraDirector.hpp"

namespace MR {
    void initCameraCodeCollection(const char* pName, s32 code) {
        MR::getCameraDirector()->initCameraCodeCollection(pName, code);
    }

    void registerCameraCode(u32 code) {
        MR::getCameraDirector()->registerCameraCode(code);
    }

    void termCameraCodeCollection() {
        MR::getCameraDirector()->termCameraCodeCollection();
    }
};  // namespace MR
