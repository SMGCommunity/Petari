#pragma once

#include "Game/Screen/WipeHolderBase.hpp"

class SceneWipeHolder : public WipeHolderBase {
public:
    /// @brief Creates a new `SceneWipeHolder`.
    SceneWipeHolder();

    /// @brief Destroys the `SceneWipeHolder`.
    virtual ~SceneWipeHolder() {}
};

namespace SceneWipeHolderFunction {
    SceneWipeHolder* getSceneWipeHolder();
    void openWipe(const char *, s32);
    void closeWipe(const char *, s32);
    void forceOpenWipe(const char *);
    void forceCloseWipe(const char *);
};
