#include "Game/Speaker/SpkData.h"



/*void SpkData::loadTable(u16 table) {

}*/

bool SpkData::isValid() const {
    return table.initialized && unk_4;
}