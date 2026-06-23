#pragma once

#include <revolution/types.h>

class IconComet;
class LayoutActor;

namespace nw4r {
    namespace lyt {
        class TexMap;
    };  // namespace lyt
};  // namespace nw4r

class GalaxyInfoLayoutSetter {
public:
    enum GalaxyState {
        GalaxyState_Hatena,
        GalaxyState_Unknown,
        GalaxyState_New,
        GalaxyState_Opened,
    };

    /// @brief Creates a new `GalaxyInfoLayoutSetter`.
    /// @param pHost A pointer to the owning actor instance.
    GalaxyInfoLayoutSetter(LayoutActor* pHost);

    void replaceTexture(const char*);
    void updateInfo(const char*, GalaxyState);
    void updateInfo(const char*, GalaxyState, int);
    void updateCometPos();
    static u32 getAnimLayerNum();
    void initReplaceTexture();
    void updateInfoWithoutComet(const char*, GalaxyState);
    void setTextBoxStarNum(const char*);
    void showCometWindow(int);
    void hideCometWindow();

    /* 0x00 */ LayoutActor* mHost;
    /* 0x04 */ nw4r::lyt::TexMap** mReplaceTexture;
    /* 0x08 */ nw4r::lyt::TexMap* mUnknownTexture;
    /* 0x0C */ IconComet* mIconComet;
};
