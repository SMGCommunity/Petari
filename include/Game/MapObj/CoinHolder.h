#pragma once

class NameObj;
class Coin;

namespace MR {
    void createCoinHolder();
    void createCoinRotater();

    void addToCoinHolder(const NameObj *, Coin *);
};