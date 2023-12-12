#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/NameObj/NameObjArchiveListCollector.h"

class LuigiLetter {
public:
    static void makeArchiveListForMenu(NameObjArchiveListCollector *, const JMapInfoIter &);
};