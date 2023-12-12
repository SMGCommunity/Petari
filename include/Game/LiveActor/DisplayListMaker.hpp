#pragma once

#include <revolution.h>

class J3DAnmBase;

class DisplayListMaker {
public:
    void onCurFlagBpk(const J3DAnmBase *);
    void offCurFlagBpk(const J3DAnmBase *);
    void onCurFlagBtp(const J3DAnmBase *);
    void offCurFlagBtp(const J3DAnmBase *);
    void onCurFlagBtk(const J3DAnmBase *);
    void offCurFlagBtk(const J3DAnmBase *);
    void onCurFlagBrk(const J3DAnmBase *);
    void offCurFlagBrk(const J3DAnmBase *);

    void newDifferedDisplayList();

    void update();

    bool isValidDiff();
};