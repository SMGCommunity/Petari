#pragma once

class NameObj;

namespace MR {
    void connectToSceneAreaObj(NameObj *);

    bool isStageStateScenarioOpeningCamera();

    void requestMovementOn(NameObj *);
    void requestMovementOff(NameObj *);
};
