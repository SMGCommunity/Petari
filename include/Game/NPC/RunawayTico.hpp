#pragma once

#include "Game/NPC/Tico.hpp"

class RunawayTico : public Tico {
public:
    RunawayTico(const char*);
    virtual ~RunawayTico();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
};
