#include "Game/Player/MatrixControl.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/HashUtil.hpp"

MatrixControl::MatrixControl(const char* pName, MatrixMap* pMap, MatrixSelectList* pSelectList, s32 myInt)
    : NameObj(pName), _C(pMap), _10(pSelectList), _18(myInt), _1C(true), _1D(false) {
    for (s32 idx = 0; _C->mMaps->mName[0] != '\x00'; idx++) {
        if (_1C) {
            continue;
        }

        _C->mMaps->_8 = 0;
    }

    s32 size;
    _14 = new HashSortTable(size);

    for (u32 idx = 0; idx < size; idx++) {
        _14->add(_C->mMaps[idx].mName, idx, false);
    }

    _14->sort();
}

u8 MatrixControl::getValue(const char* pName, u8 value) const {
    u32 index;
    _14->search(pName, &index);
    return _C->mMaps[index]._8[value];
}

bool MatrixControl::getValueOrNone(const char* pName, u8 value, u8* newValue) const {
    u32 index;
    if (!_14->search(pName, &index)) {
        return false;
    }

    if (newValue != nullptr) {
        *newValue = _C->mMaps[index]._8[value];
    }

    return true;
}

bool MatrixControl::getBit(const char* pName, u8 bit) const {
    u32 index;
    _14->search(pName, &index);
    return _C->mMaps[index]._4 & (1 << (31 - bit));
}

bool MatrixControl::isExist(const char* pName) const {
    u32 index;
    return _14->search(pName, &index);
}

bool MatrixControl::getBitOrNone(const char* pName, u8 bit) const {
    // FIXME: some black magickery going on here
    u32 index;
    if (_14->search(pName, &index)) {
        return _1D;
    }

    return _C->mMaps[index]._4 & (1 << (31 - bit));
}

MatrixValueGetter::MatrixValueGetter(const char* pName, MatrixValueTable* pTable) : NameObj(pName), _C(pTable) {
    s32 size;
    for (size = 0; _C->mValues[size].mName[0] != '\x00'; size++)
        ;

    _10 = new HashSortTable(size);

    for (u32 idx = 0; idx < size; idx++) {
        _10->add(_C->mValues[idx].mName, idx, false);
        _10->sort();
    }
}

bool MatrixValueGetter::getValue(const char* pName, f32* value) const {
    u32 index;
    if (!_10->search(pName, &index)) {
        return false;
    }

    if (value != nullptr) {
        *value = _C->mValues[index].mValue;
    }

    return true;
}

MatrixControl::~MatrixControl() {
}

MatrixValueGetter::~MatrixValueGetter() {
}
