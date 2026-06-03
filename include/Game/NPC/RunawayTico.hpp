#pragma once

#include "Game/NPC/Tico.hpp"
#include "Game/NameObj/NameObj.hpp"

class NameObjArchiveListCollector;
class JMapInfoIter;

class RunawayTico : public Tico {
public:
    RunawayTico(const char*);
    virtual ~RunawayTico();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
};
