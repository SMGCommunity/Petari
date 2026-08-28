#pragma once

#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>

namespace nw4r {
    namespace lyt {
        class AnimTransform;
    };  // namespace lyt
};  // namespace nw4r

class LayoutManager;

class LayoutAnmPlayer {
public:
    LayoutAnmPlayer(const LayoutManager*);

    /* 0x08 */ virtual void movement();
    /* 0x0C */ virtual void reflectFrame();
    /* 0x10 */ virtual void start(const char*);
    /* 0x14 */ virtual void stop();

    bool isStop() const;

    /* 0x04 */ const char* mAnimName;
    /* 0x08 */ nw4r::lyt::AnimTransform* mAnimTransform;
    /* 0x0C */ const LayoutManager* mManager;
    /* 0x10 */ J3DFrameCtrl mFrameCtrl;
};
