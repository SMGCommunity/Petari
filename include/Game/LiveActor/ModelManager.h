#pragma once

#include "Game/Animation/AnmPlayer.h"

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
    
    void calc();
    
    J3DModel* getJ3DModel() const;

    void changeBckSetting(const char *, const char *, XanimePlayer *);

    BtkPlayer* mBtkPlayer;                  // _0
    BrkPlayer* mBrkPlayer;                  // _4
    BtpPlayer* mBtpPlayer;                  // _8
    BpkPlayer* mBpkPlayer;                  // _C
    BvaPlayer* mBvaPlayer;                  // _10
    XanimeResourceTable* mResourceTable;    // _14
    XanimePlayer* mXanimePlayer;            // _18
    J3DModel* mModel;                       // _1C
    ResourceHolder* mModelResourceHolder;   // _20
    DisplayListMaker* mDisplayListMaker;    // _24
};