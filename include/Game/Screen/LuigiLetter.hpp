#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"

class LuigiLetter {
public:
    static void makeArchiveListForMenu(NameObjArchiveListCollector *, const JMapInfoIter &);
};