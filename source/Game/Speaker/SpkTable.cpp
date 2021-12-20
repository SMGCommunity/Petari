#include "Game/Speaker/SpkTable.h"

SpkTable::SpkTable() {
    initialized = false;
    resource = NULL;
    unk_1 = 0;
    unk_2 = 0;
}

void SpkTable::setResource(void* resource) {
    initialized = true;
    this->resource = resource;
    // TODO!!!
}