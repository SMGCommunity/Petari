#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NPC/Tico.hpp"

class NameObjArchiveListCollector;
class JMapInfoIter;

class RunawayTico : public Tico {
public:
    RunawayTico(const char*);
    virtual ~RunawayTico();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
};
