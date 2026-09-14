#include "Game/Player/MatrixControl.hpp"
#include "Game/Util/HashUtil.hpp"

MatrixControl::MatrixControl(const char* pName, MatrixMap* pMap, MatrixSelectList* pSelectList, s32 selectCount)
    : NameObj(pName), _C(pMap), _10(pSelectList), _18(selectCount), _1C(true), _1D() {
    for (u32 i = 0; i < selectCount; i++) {
        if (_10[i + 1].mCount > 2) {
            _1C = false;
            break;
        }
    }

    u32 size;
    for (size = 0; _C[size].mName[0] != '\0'; size++) {
        if (!_1C) {
            _C[size]._8 = new u8[8];

            for (u32 i = 0; i < 8; i++) {
                _C[size]._8[i] = (_C[size]._4 >> ((7 - i) * 4)) & 0xF;
            }
        }
    }

    _14 = new HashSortTable(size);

    for (u32 i = 0; i < size; i++) {
        _14->add(_C[i].mName, i, false);
    }

    _14->sort();
}

u8 MatrixControl::getValue(const char* pName, u8 value) const {
    u32 index;
    _14->search(pName, &index);
    return _C[index]._8[value];
}

bool MatrixControl::getValueOrNone(const char* pName, u8 idx, u8* pValue) const {
    u32 index;
    if (!_14->search(pName, &index)) {
        return false;
    }

    if (pValue != nullptr) {
        *pValue = _C[index]._8[idx];
    }

    return true;
}

bool MatrixControl::getBit(const char* pName, u8 bit) const {
    u32 index;
    _14->search(pName, &index);
    return _C[index]._4 & (1 << (31 - bit));
}

bool MatrixControl::isExist(const char* pName) const {
    u32 index;
    return _14->search(pName, &index);
}

bool MatrixControl::getBitOrNone(const char* pName, u8 bit) const {
    u32 index;
    if (_14->search(pName, &index) == false) {
        return _1D;
    }

    return _C[index]._4 & (1 << (31 - bit));
}

MatrixValueGetter::MatrixValueGetter(const char* pName, MatrixValueTable* pTable) : NameObj(pName), _C(pTable) {
    s32 size;
    for (size = 0; _C[size].mName[0] != '\x00'; size++)
        ;

    _10 = new HashSortTable(size);

    for (u32 idx = 0; idx < size; idx++) {
        _10->add(_C[idx].mName, idx, false);
        _10->sort();
    }
}

bool MatrixValueGetter::getValue(const char* pName, f32* pValue) const {
    u32 index;
    if (!_10->search(pName, &index)) {
        return false;
    }

    if (pValue != nullptr) {
        *pValue = _C[index].mValue;
    }

    return true;
}

MatrixControl::~MatrixControl() {
}

MatrixValueGetter::~MatrixValueGetter() {
}
