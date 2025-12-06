#pragma once

class MultiEmitter;

class PaneEffectKeeper {
public:
    void clear();
    MultiEmitter* getEmitter(const char*) const;
};
