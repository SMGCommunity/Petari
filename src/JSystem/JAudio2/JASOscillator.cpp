#include "JSystem/JAudio2/JASOscillator.hpp"

f32 const JASOscillator::sCurveTableLinear[17] = {
    1.0f, 0.9375f, 0.875f, 0.8125f, 0.75f, 0.6875f, 0.625f, 0.5625f, 0.5f, 0.4375f, 0.375f, 0.3125f, 0.25f, 0.1875f, 0.125f, 0.0625f, 0.0f,
};

f32 const JASOscillator::sCurveTableSampleCell[17] = {1.0f,
                                                      0.9704890251159668f,
                                                      0.7812740206718445f,
                                                      0.5462809801101685f,
                                                      0.39979198575019836f,
                                                      0.28931498527526855f,
                                                      0.21210399270057678f,
                                                      0.15747599303722382f,
                                                      0.1126129999756813f,
                                                      0.08178959786891937f,
                                                      0.057985201478004456f,
                                                      0.04364150017499924f,
                                                      0.03082370012998581f,
                                                      0.023712899535894394f,
                                                      0.015259300358593464f,
                                                      0.00915555004030466f,
                                                      0.0};

f32 const JASOscillator::sCurveTableSqRoot[17] = {1.0f,      0.8789060115814209f,  0.765625f, 0.6601560115814209f, 0.5625f,   0.4726560115814209f,
                                                  0.390625f, 0.3164060115814209f,  0.25f,     0.1914059966802597f, 0.140625f, 0.09765619784593582f,
                                                  0.0625f,   0.03515620157122612f, 0.015625f, 0.00390625f,         0.0f};

f32 const JASOscillator::sCurveTableSquare[17] = {
    1.0f,
    0.9682459831237793f,
    0.9354140162467957f,
    0.9013879895210266f,
    0.8660249710083008f,
    0.8291559815406799f,
    0.790569007396698f,
    0.75f,
    0.7071070075035095f,
    0.66143798828125f,
    0.6123719811439514f,
    0.55901700258255f,
    0.5f,
    0.433012992143631f,
    0.35355299711227417f,
    0.25f,
    0.0f,
};

JASOscillator::JASOscillator() {
    mData = nullptr;
    _14 = 0;
    mDirectRelease = 0;
    _18 = 0;
    _1C = 0;
    _04 = _08 = _10 = _0C = 0.0f;
}

void JASOscillator::initStart(JASOscillator::Data const* data) {
    mData = data;
    _04 = 0.0f;
    _08 = 0.0f;
    _0C = 0.0f;
    _14 = 0;
    mDirectRelease = 0;
    if (!data) {
        _1C = 0;
        return;
    }

    if (mData->mTable == nullptr) {
        _1C = 2;
        _08 = 1.0f;
        return;
    }

    _10 = mData->mTable[0]._4 / 32768.0f;
    _18 = mData->mTable[0]._0;
    _1C = 1;
}

void JASOscillator::incCounter(f32 param_0) {
    const short* v1;
    switch (_1C) {
    case 0:
    case 2:
        return;
    case 1:
        break;
    }
    _04 += param_0 * mData->_04;
    update();
}

f32 JASOscillator::getValue() const {
    return _08 * mData->mScale + mData->_14;
}

void JASOscillator::release() {
    if (mData->rel_table == nullptr && mDirectRelease == 0) {
        stop();
        return;
    }

    if (mDirectRelease != 0) {
        _04 = 0.0f;
        _0C = _08;
        _10 = 0.0f;
        _14 = 0;
        _18 = (mDirectRelease >> 14) & 3;
        _1C = 4;
        update();
        return;
    }

    if (mData->mTable != mData->rel_table) {
        _04 = 0.0f;
        _0C = _08;
        _10 = mData->rel_table[0]._4 / 32768.0f;
        _14 = 0;
        _18 = mData->rel_table[0]._0;
    }

    _1C = 3;
    update();
}

void JASOscillator::update() {
    if (_1C == 4) {
        s32 x = mDirectRelease & 0x3FFF;
        if (_04 < x) {
            updateCurrentValue(x);
        } else {
            _08 = _10;
            _1C = 0;
        }
        return;
    }

    if (_1C == 2)
        return;

    const Point* psVar1 = _1C == 1 ? mData->mTable : mData->rel_table;

    if (psVar1 == nullptr) {
        _1C = 2;
        return;
    }

    while (_04 >= psVar1[_14]._2) {
        _04 -= psVar1[_14]._2;
        _08 = _10;
        _14++;
        _0C = _08;
        const s16* ps = &psVar1[_14]._0;
        s16 r26 = ps[0];
        switch (r26) {
        case 0xf:
            _1C = 0;
            return;
        case 0xe:
            _1C = 2;
            return;
        case 0xd:
            _14 = ps[2];
            break;
        default:
            _18 = r26;
            _10 = ps[2] / 32768.0f;
            break;
        }
    }

    updateCurrentValue(psVar1[_14]._2);
}

void JASOscillator::updateCurrentValue(f32 param_0) {
    static f32* table_list[4] = {
        (f32*)JASOscillator::sCurveTableLinear,
        (f32*)JASOscillator::sCurveTableSquare,
        (f32*)JASOscillator::sCurveTableSqRoot,
        (f32*)JASOscillator::sCurveTableSampleCell,
    };
    f32* table = table_list[_18];
    f32 fVar1 = 16.0f * (_04 / param_0);
    u32 index = (u32)fVar1;
    f32 fVar3 = (fVar1 - index);
    f32 fVar4 = (1.0f - fVar3) * table[index] + fVar3 * table[index + 1];
    _08 = _0C * fVar4 + _10 * (1.0f - fVar4);
}
