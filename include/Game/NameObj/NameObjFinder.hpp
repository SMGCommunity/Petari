#pragma once

class NameObj;

/// @brief Class that supplies a static function to find a NameObj contained in the global NameObjHolder.
class NameObjFinder {
public:
    static NameObj* find(const char*);
};
