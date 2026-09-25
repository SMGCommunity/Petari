#include "Game/Scene/SceneFactory.hpp"
#include "Game/Scene/GameScene.hpp"
#include "Game/Scene/IntermissionScene.hpp"
#include "Game/Scene/LogoScene.hpp"
#include <algorithm>

namespace {
    struct Name2CreateFunc {
        bool operator==(const char* pName) const {
            return strcmp(mName, pName) == 0;
        }

        /* 0x0 */ const char* mName;
        /* 0x4 */ Scene* (*mCreateFunc)();
    };

    template < typename T >
    Scene* createScene() {
        return new T();
    }

    const Name2CreateFunc cCreateTable[] = {
        {"Game", createScene< GameScene >},
        {"Intermission", createScene< IntermissionScene >},
        {"Logo", createScene< LogoScene >},
    };
};  // namespace

namespace MR {
    Scene* createScene(const char* pName) {
        const Name2CreateFunc* pIter = std::find(::cCreateTable, ::cCreateTable + ARRAY_SIZE(::cCreateTable), pName);

        if (pIter == &::cCreateTable[ARRAY_SIZE(::cCreateTable)]) {
            return nullptr;
        }

        return (*pIter->mCreateFunc)();
    }
};  // namespace MR
