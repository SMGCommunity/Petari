#pragma once

#include "Game/NPC/Tico.hpp"

class TicoAstro : public Tico {
public:
    TicoAstro(const char*);

    /* 0x08 */ virtual ~TicoAstro();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
};
