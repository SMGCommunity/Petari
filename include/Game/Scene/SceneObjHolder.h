#pragma once

#define SceneObj_PlanetMapCreator 0x74

class NameObj;

class SceneObjHolder {
public:
    NameObj* getObj(int) const;
};

namespace MR {
    SceneObjHolder* getSceneObjHolder();
};
