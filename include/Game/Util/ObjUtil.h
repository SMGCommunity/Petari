#pragma once

class NameObj;
class JMapInfo;

namespace MR {
    void connectToSceneAreaObj(NameObj *);

    bool isStageStateScenarioOpeningCamera();

    void requestMovementOn(NameObj *);
    void requestMovementOff(NameObj *);
    
    const JMapInfo* createCsvParser(const char *, const char *, ...);
    s32 getCsvDataElementNum(const JMapInfo *);

    void getCsvDataStr(const char **, const JMapInfo *, const char *, s32);
};
