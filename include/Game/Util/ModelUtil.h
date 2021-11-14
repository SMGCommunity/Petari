#pragma once

class J3DModel;
class LiveActor;

namespace MR {
    bool isExistModel(const char *);

    J3DModel* getJ3DModel(const LiveActor *);
};