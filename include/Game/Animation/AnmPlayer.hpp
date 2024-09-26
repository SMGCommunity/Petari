#pragma once

#include "JSystem/J3DGraphAnimator/J3DAnimation.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"
#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"

class J3DAnmBase;
class ResTable;
class ResourceHolder;

class AnmPlayerBase {
public:
    AnmPlayerBase(const ResTable *);

    virtual void changeAnimation(J3DAnmBase *);
    virtual void stopAnimation();

    void update();
    void reflectFrame();
    void start(const char *);
    void stop();
    bool isPlaying(const char *) const;
    bool isStop() const;

    void* mData;                        // 0x4
    const ResTable* mResourceTable;     // 0x8
    J3DFrameCtrl mFrameCtrl;            // 0xC
};

class MaterialAnmPlayerBase : public AnmPlayerBase {
public:
    MaterialAnmPlayerBase(const ResTable *, J3DModelData *);

    virtual void attach(J3DAnmBase *, J3DModelData *) = 0;
    virtual void detach(J3DAnmBase *, J3DModelData *) = 0;

    void beginDiff();
    void endDiff();

    J3DModelData* mModelData;   // 0x20
};

class BpkPlayer : public MaterialAnmPlayerBase {
public:
    BpkPlayer(const ResourceHolder *, J3DModelData *);

    virtual void attach(J3DAnmBase *, J3DModelData *);
    virtual void detach(J3DAnmBase *, J3DModelData *);
};

class BrkPlayer : public MaterialAnmPlayerBase {
public:
    BrkPlayer(const ResourceHolder *, J3DModelData *);

    virtual void attach(J3DAnmBase *, J3DModelData *);
    virtual void detach(J3DAnmBase *, J3DModelData *);
};

class BtkPlayer : public MaterialAnmPlayerBase {
public:
    BtkPlayer(const ResourceHolder *, J3DModelData *);

    virtual void attach(J3DAnmBase *, J3DModelData *);
    virtual void detach(J3DAnmBase *, J3DModelData *);
};

class BtpPlayer : public MaterialAnmPlayerBase {
public:
    BtpPlayer(const ResourceHolder *, J3DModelData *);

    virtual void attach(J3DAnmBase *, J3DModelData *);
    virtual void detach(J3DAnmBase *, J3DModelData *);
};

class BvaPlayer : public AnmPlayerBase {
public:
    BvaPlayer(const ResTable *, J3DModel *);

    void calc();
    bool getAnmVisibility();

    J3DModel* mModel;   // 0x20
};
