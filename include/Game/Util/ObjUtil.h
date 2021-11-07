#pragma once

class NameObj;
class JMapInfo;
class ResourceHolder;

namespace MR {
    void connectToScene(NameObj *, int, int, int, int);
    void connectToSceneAreaObj(NameObj *);
    void connectToSceneMapObjMovement(NameObj *);

    bool isStageStateScenarioOpeningCamera();

    void requestMovementOn(NameObj *);
    void requestMovementOff(NameObj *);
    
    const JMapInfo* createCsvParser(const char *, const char *, ...);
    s32 getCsvDataElementNum(const JMapInfo *);

    void getCsvDataS32(s32 *, const JMapInfo *, const char *, s32);
    void getCsvDataStr(const char **, const JMapInfo *, const char *, s32);
    void getCsvDataBool(bool *, const JMapInfo *, const char *, s32);

    ResourceHolder* createAndAddResourceHolder(const char *);
};
