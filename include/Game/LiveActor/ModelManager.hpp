#pragma once

#include "Game/Animation/AnmPlayer.hpp"

class BckCtrl;
class BrkCtrl;
class DisplayListMaker;
class ResourceHolder;
class XanimePlayer;
class XanimeResourceTable;

class ModelManager {
public:
    ModelManager();

    void init(const char *, const char *, bool);

    void update();
    void calcAnim();
    void calcView();
    void entry();
    void newDifferedDLBuffer();
    void updateDL(bool);
    void startBck(const char *, const char *);
    void startBckWithInterpole(const char *, s32);
    void startBtk(const char *);
    void startBrk(const char *);
    void startBtp(const char *);
    void startBpk(const char *);
    void startBva(const char *);
    void stopBtk();
    void stopBrk();
    void stopBtp();
    void stopBpk();
    void stopBva();
    BckCtrl* getBckCtrl() const;
    BrkCtrl* getBrkCtrl() const;

    bool isBckStopped() const;
    bool isBtkStopped() const;
    bool isBrkStopped() const;
    bool isBtpStopped() const;
    bool isBpkStopped() const;
    bool isBvaStopped() const;
    bool isBtkPlaying(const char *) const;
    bool isBrkPlaying(const char *) const;
    bool isBpkPlaying(const char *) const;
    bool isBtpPlaying(const char *) const;
    bool isBvaPlaying(const char *) const;
    void initJointTransform();

    ResourceHolder* getModelResourceHolder() const;

    J3DModelData* getJ3DModelData() const;
    
    void calc();
    
    J3DModel* getJ3DModel() const;

    void initVisibilityAnm();

    void changeBckSetting(const char *, const char *, XanimePlayer *);

    BtkPlayer* mBtkPlayer;                  // 0x0
    BrkPlayer* mBrkPlayer;                  // 0x4
    BtpPlayer* mBtpPlayer;                  // 0x8
    BpkPlayer* mBpkPlayer;                  // 0xC
    BvaPlayer* mBvaPlayer;                  // 0x10
    XanimeResourceTable* mResourceTable;    // 0x14
    XanimePlayer* mXanimePlayer;            // 0x18
    J3DModel* mModel;                       // 0x1C
    ResourceHolder* mModelResourceHolder;   // 0x20
    DisplayListMaker* mDisplayListMaker;    // 0x24
};