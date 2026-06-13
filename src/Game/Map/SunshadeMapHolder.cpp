#include "Game/Map/SunshadeMapHolder.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/MapUtil.hpp"

SunshadeMapHolder::SunshadeMapHolder() : NameObj("日よけコリジョン管理") {
    _C.x = 0.0f;
    _C.y = 1.0f;
    _C.z = 0.0f;
}

namespace MR {
    bool isInShadeFromTheSun(const TVec3f& rPos, f32 a2) {
        if (!MR::isInAreaObj("SunLightArea", rPos)) {
            return true;
        }

        if (!MR::isExistSceneObj(SceneObj_SunshadeMapHolder)) {
            return false;
        }

        SunshadeMapHolder* holder = MR::getSceneObj< SunshadeMapHolder >(SceneObj_SunshadeMapHolder);

        return Collision::checkStrikeLineToSunshade(rPos, holder->_C.multInLine(a2), 0, nullptr, nullptr) != 0;
    }

    void createSunshadeMapHolder() {
        MR::createSceneObj(SceneObj_SunshadeMapHolder);
    }
};  // namespace MR

SunshadeMapHolder::~SunshadeMapHolder() {
}
