#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include <nw4r/lyt/layout.h>

namespace MR {
    GameSystemObjHolder* getGameSystemObjHolder() {
        return SingletonHolder<GameSystem>::get()->mObjHolder;
    }
};

namespace {
    FunctionAsyncExecutor* getFunctionAsyncExecutor() {
        return SingletonHolder<GameSystem>::get()->mObjHolder->mAsyncExec;
    }
};

namespace {
    NameObjHolder* getSceneNameObjHolder() {
        return SingletonHolder<GameSystem>::get()->mSceneController->mObjHolder;
    }
};

namespace MR {
    void setLayoutDefaultAllocator() {
        nw4r::lyt::Layout::mspAllocator = &MR::NewDeleteAllocator::sAllocator;
    }

    bool isDisplayEncouragePal60Window() {
        return VIGetTvFormat() == 1;
    }
};  
