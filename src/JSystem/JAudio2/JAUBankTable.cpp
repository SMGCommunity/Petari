#include "JSystem/JAudio2/JAUBankTable.hpp"

void JAUBankTableDictionary::appendBankTable(JSULink< JAUBankTable >* bankTableList) {
    append(bankTableList);
}

// Needed to make the inline function JAUBankTable::getBank appear in this TU
static const JASBank* dummy(const JAUBankTable* table) {
    return table->getBank(0);
}
