#pragma once

class ModelManager {
public:
    ModelManager();

    void update();
    void calcAnim();
    void calcView();
    void entry();
    void newDifferedDLBuffer();
    void updateDL(bool);
    void startBck(const char *, const char *);
    void startBckWithInterpole(const char *, s32);
    void startBrk(const char *);
    void startBtp(const char *);
    void startBpk(const char *);
};