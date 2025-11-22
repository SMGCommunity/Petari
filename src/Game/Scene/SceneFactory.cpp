#include "Game/Scene/SceneFactory.hpp"
#include "Game/Scene/GameScene.hpp"
#include "Game/Scene/IntermissionScene.hpp"
#include "Game/Scene/LogoScene.hpp"
#include <cstdio>

namespace {
    struct Name2CreateFunc {
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
        const Name2CreateFunc* pBegin = &cCreateTable[0];
        const Name2CreateFunc* pEnd = &cCreateTable[sizeof(cCreateTable) / sizeof(*cCreateTable)];
        const Name2CreateFunc* pIter;

        for (pIter = pBegin; pIter != pEnd; pIter++) {
            bool isEqualName = strcmp(pIter->mName, pName) == 0;

            if (!isEqualName) {
                break;
            }
        }

        if (pIter == pEnd) {
            return nullptr;
        }

        return (*pIter->mCreateFunc)();
    }
};  // namespace MR
