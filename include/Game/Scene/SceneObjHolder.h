#pragma once

#define SceneObj_LightDirector          0x6
#define SceneObj_NameObjExecuteHolder   0x58
#define SceneObj_PlanetMapCreator       0x74

class NameObj;

class SceneObjHolder {
public:
    NameObj* getObj(int) const;
};

namespace MR {
    SceneObjHolder* getSceneObjHolder();
};
