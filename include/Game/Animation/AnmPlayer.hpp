#pragma once

#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>

class J3DAnmBase;
class J3DModelData;
class ResTable;
class ResourceHolder;

class AnmPlayerBase {
public:
    AnmPlayerBase(const ResTable*);

    virtual void changeAnimation(J3DAnmBase*);
    virtual void stopAnimation();

    void update();
    void reflectFrame();
    void start(const char*);
    void stop();
    bool isPlaying(const char*) const;
    bool isStop() const;

    /* 0x4 */ J3DAnmBase* mAnmRes;
    /* 0x8 */ const ResTable* mResTable;
    /* 0xC */ J3DFrameCtrl mFrameCtrl;
};

class MaterialAnmPlayerBase : public AnmPlayerBase {
public:
    MaterialAnmPlayerBase(const ResTable*, J3DModelData*);

    virtual void attach(J3DAnmBase*, J3DModelData*) = 0;
    virtual void detach(J3DAnmBase*, J3DModelData*) = 0;

    void beginDiff();
    void endDiff();

    /* 0x20 */ J3DModelData* mModelData;
};
