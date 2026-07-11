#pragma once

#include "Game/NameObj/NameObj.hpp"

class MatrixValueTable;
class MatrixMap;
class MatrixSelectList;
class HashSortTable;

struct Map {
    /* 0x0 */ const char* mName;
    /* 0x4 */ u32 _4;
    /* 0x8 */ u8* _8;
};

struct MatrixMap {
    Map mMaps[];
};

struct MatrixSelectList {
    /* 0x0 */ u8 _0[0x84];
};

struct MatrixValue {
    /* 0x0 */ const char* mName;
    /* 0x4 */ f32 mValue;
};

struct MatrixValueTable {
    MatrixValue mValues[];
};

class MatrixControl : public NameObj {
public:
    MatrixControl(const char*, MatrixMap*, MatrixSelectList*, s32);

    virtual ~MatrixControl();

    u8 getValue(const char*, u8) const;
    bool getValueOrNone(const char*, u8, u8*) const;
    bool getBit(const char*, u8) const;
    bool isExist(const char*) const;
    bool getBitOrNone(const char*, u8) const;

    /* 0x0C */ MatrixMap* _C;
    /* 0x10 */ MatrixSelectList* _10;
    /* 0x14 */ HashSortTable* _14;
    /* 0x18 */ s32 _18;
    /* 0x1C */ bool _1C;
    /* 0x1D */ bool _1D;
};

class MatrixValueGetter : public NameObj {
public:
    MatrixValueGetter(const char*, MatrixValueTable*);

    virtual ~MatrixValueGetter();

    bool getValue(const char*, f32*) const;

    /* 0x0C */ MatrixValueTable* _C;
    /* 0x10 */ HashSortTable* _10;
};
