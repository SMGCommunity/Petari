#pragma once

#include "Game/Screen/LayoutActor.hpp"

namespace nw4r {
    namespace lyt {
        class TexMap;
    };
}; // namespace nw4r

class ErrorMessageWindow : public LayoutActor {
public:
    enum MessageType {
        MessageType_System,
        MessageType_Game,
    };

    /// @brief Creates a new `ErrorMessageWindow`.
    ErrorMessageWindow();

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();

    bool isAnimEnd() const;
    void appearWithMessage(const char*, MessageType, const nw4r::lyt::TexMap*);
    void disappear();
    void exeHide();
    void exeAppear();
    void exeDisplay();
    void exeDisappear();
    void exeDisappearBeforeAppear();

private:
    /* 0x20 */ MessageType              mMessageType;
    /* 0x24 */ const char*              mMessageId;
    /* 0x28 */ const nw4r::lyt::TexMap* mTexture;
};
