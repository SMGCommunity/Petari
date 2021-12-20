#include "Game/Speaker/SpkWave.h"

SpkWave::SpkWave() {
    resource = NULL;
}

void SpkWave::setResource(void *resource) {
    OSDisableInterrupts();
    this->resource = resource;
    OSEnableInterrupts();
}

u32 SpkWave::getWaveSize(long wave) const {
    if (resource == NULL) {
        return 0;
    } else {
        return *getWaveData(wave);
    }
}

u32* SpkWave::getWaveData(long wave) const {
    //u32* off = &resource[wave];
    //return resource + *off; TODO!!!
    return 0;
}