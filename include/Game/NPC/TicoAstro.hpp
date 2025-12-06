#pragma once

#include "Game/NPC/Tico.hpp"

class TicoAstro : public Tico {
public:
    TicoAstro(const char*);

    virtual ~TicoAstro();
    virtual void init(const JMapInfoIter&);
};
